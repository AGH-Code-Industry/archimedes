#include <algorithm>
#include <format>
#include <stacktrace>
#include <string>
#include <utility>

#include <archimedes/BuildInfo.h>
#include <archimedes/Exception.h>
#include <archimedes/exceptions/ConfigException.h>
#include <archimedes/utils/SourceAwareFormatString.h>
#include <gtest/gtest.h>

namespace {

constexpr std::string_view title = "Test";
constexpr std::string_view message = "boom";
constexpr std::string_view banner = "[Test]: boom";
constexpr std::string_view entryMarker = "    at ";

size_t countOccurrences(const std::string_view value, const std::string_view needle) {
	size_t count = 0;
	for (size_t pos = 0; (pos = value.find(needle, pos)) != std::string_view::npos; pos += needle.size()) {
		++count;
	}
	return count;
}

void expectLocationState(const arch::Exception& exception) {
	EXPECT_FALSE(exception.hasStacktrace());
	EXPECT_TRUE(exception.location());
	EXPECT_FALSE(exception.stacktrace());
}

void expectStacktraceState(const arch::Exception& exception, const size_t expectedSize) {
	EXPECT_TRUE(exception.hasStacktrace());
	EXPECT_FALSE(exception.location());
	ASSERT_TRUE(exception.stacktrace());
	EXPECT_EQ(exception.stacktrace()->size(), expectedSize);
}

arch::Exception makeLocationException() {
	return { title, message };
}

arch::Exception makeStacktraceException(const std::stacktrace& stacktrace) {
	return arch::Exception(title, message).withStacktrace(stacktrace);
}

template<class... Args>
arch::utils::SimpleSourceLocation captureFormatLocation(
	arch::utils::SourceAwareFormatString<Args...> format,
	Args&&...
) {
	return format.loc;
}

} // namespace

TEST(SimpleSourceLocationTest, CapturesCallSite) {
	const auto expectedLine = __LINE__ + 1;
	const auto location = arch::utils::SimpleSourceLocation::current();

	EXPECT_EQ(location.line(), expectedLine);
	EXPECT_STREQ(location.fileName(), __FILE__);
	EXPECT_STREQ(location.shortFunctionName(), "TestBody");
	EXPECT_NE(std::string_view(location.functionName()).find("SimpleSourceLocationTest"), std::string_view::npos);
}

TEST(SourceAwareFormatStringTest, CapturesCallSite) {
	const auto expectedLine = __LINE__ + 1;
	const auto location = captureFormatLocation("value {}", 42);

	EXPECT_EQ(location.line(), expectedLine);
	EXPECT_STREQ(location.fileName(), __FILE__);
	EXPECT_STREQ(location.shortFunctionName(), "TestBody");
	EXPECT_NE(std::string_view(location.functionName()).find("SourceAwareFormatStringTest"), std::string_view::npos);
}

TEST(ExceptionTest, StoresSourceLocationByDefault) {
	const auto location = arch::utils::SimpleSourceLocation::current();
	const arch::Exception exception(title, message, location);

	expectLocationState(exception);
	ASSERT_TRUE(exception.location());
	EXPECT_EQ(exception.location()->line(), location.line());
	EXPECT_EQ(exception.location()->column(), location.column());
	EXPECT_STREQ(exception.location()->fileName(), location.fileName());
	EXPECT_STREQ(exception.location()->functionName(), location.functionName());
}

TEST(ExceptionTest, DerivedExceptionForwardsConstructionCallSite) {
	const auto expectedLine = __LINE__ + 1;
	const arch::ConfigException exception("boom");

	expectLocationState(exception);
	ASSERT_TRUE(exception.location());
	EXPECT_EQ(exception.location()->line(), expectedLine);
	EXPECT_STREQ(exception.location()->fileName(), __FILE__);
}

TEST(ExceptionTest, FormatsOnlyBannerWhenLocationWasNotRequested) {
	const arch::Exception exception(title, message);

	EXPECT_EQ(std::format("{}", exception), banner);
}

TEST(ExceptionTest, FormatsOneSourceLocationRegardlessOfRequestedDepth) {
	const auto location = arch::utils::SimpleSourceLocation::current();
	const arch::Exception exception(title, message, location);

	for (const auto& formatted : {
			 std::format("{:0}", exception),
			 std::format("{:1}", exception),
			 std::format("{:2}", exception),
		 }) {
		EXPECT_EQ(countOccurrences(formatted, entryMarker), 1);
		EXPECT_NE(formatted.find(location.fileName()), std::string::npos);
		EXPECT_NE(formatted.find(std::to_string(location.line())), std::string::npos);
	}
}

TEST(ExceptionTest, WithStacktraceSelectsExpectedStorageForBuildType) {
	const auto location = arch::utils::SimpleSourceLocation::current();
	const auto stacktrace = std::stacktrace::current();
	auto exception = arch::Exception(title, message, location);

	if constexpr (arch::buildinfo::Type::current == arch::buildinfo::Type::Release) {
		arch::log::init({ .name = "ExceptionTest", .level = arch::log::critical, .file = false });
	}
	exception.withStacktrace(stacktrace);

	if constexpr (arch::buildinfo::Type::current == arch::buildinfo::Type::Release) {
		expectLocationState(exception);
		ASSERT_TRUE(exception.location());
		EXPECT_EQ(exception.location()->line(), location.line());
	} else {
		expectStacktraceState(exception, stacktrace.size());
	}
}

TEST(ExceptionTest, FormatsRequestedNumberOfStacktraceEntries) {
	if constexpr (arch::buildinfo::Type::current == arch::buildinfo::Type::Release) {
		GTEST_SKIP() << "Release builds intentionally retain the source location";
	} else {
		const auto stacktrace = std::stacktrace::current();
		const auto exception = makeStacktraceException(stacktrace);

		EXPECT_EQ(countOccurrences(std::format("{:0}", exception), entryMarker), stacktrace.size());
		EXPECT_EQ(
			countOccurrences(std::format("{:1}", exception), entryMarker),
			std::min<size_t>(stacktrace.size(), 1)
		);
		EXPECT_EQ(
			countOccurrences(std::format("{:2}", exception), entryMarker),
			std::min<size_t>(stacktrace.size(), 2)
		);
	}
}

TEST(ExceptionTest, CopyAndMoveConstructionPreserveLocationState) {
	const auto source = makeLocationException();
	const arch::Exception copied(source);
	auto moveSource = makeLocationException();
	const arch::Exception moved(std::move(moveSource));

	expectLocationState(copied);
	expectLocationState(moved);
}

TEST(ExceptionTest, CopyAndMoveConstructionPreserveStacktraceState) {
	if constexpr (arch::buildinfo::Type::current == arch::buildinfo::Type::Release) {
		GTEST_SKIP() << "Stacktraces are unavailable in Release builds";
	} else {
		const auto stacktrace = std::stacktrace::current();
		const auto source = makeStacktraceException(stacktrace);
		const arch::Exception copied(source);
		auto moveSource = makeStacktraceException(stacktrace);
		const arch::Exception moved(std::move(moveSource));

		expectStacktraceState(copied, stacktrace.size());
		expectStacktraceState(moved, stacktrace.size());
	}
}

TEST(ExceptionTest, CopyAssignmentCanChangeActiveStorage) {
	if constexpr (arch::buildinfo::Type::current == arch::buildinfo::Type::Release) {
		GTEST_SKIP() << "Stacktraces are unavailable in Release builds";
	} else {
		const auto stacktrace = std::stacktrace::current();
		const auto locationSource = makeLocationException();
		const auto stacktraceSource = makeStacktraceException(stacktrace);
		auto target = makeLocationException();

		target = stacktraceSource;
		expectStacktraceState(target, stacktrace.size());

		target = locationSource;
		expectLocationState(target);
	}
}

TEST(ExceptionTest, MoveAssignmentCanChangeActiveStorage) {
	if constexpr (arch::buildinfo::Type::current == arch::buildinfo::Type::Release) {
		GTEST_SKIP() << "Stacktraces are unavailable in Release builds";
	} else {
		const auto stacktrace = std::stacktrace::current();
		auto target = makeLocationException();

		target = makeStacktraceException(stacktrace);
		expectStacktraceState(target, stacktrace.size());

		target = makeLocationException();
		expectLocationState(target);
	}
}

TEST(ExceptionTest, ReplacingAnExistingStacktraceIsSafe) {
	if constexpr (arch::buildinfo::Type::current == arch::buildinfo::Type::Release) {
		GTEST_SKIP() << "Stacktraces are unavailable in Release builds";
	} else {
		const auto first = std::stacktrace::current();
		const auto second = std::stacktrace::current();
		auto exception = makeStacktraceException(first);

		exception.withStacktrace(second);

		expectStacktraceState(exception, second.size());
	}
}
