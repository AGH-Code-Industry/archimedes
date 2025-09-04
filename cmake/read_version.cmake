include_guard()

# yyyy.mm.dd: 4(y) + 4(m/d) + 2(.) = 10 chars
file(READ "version.txt" ARCHIMEDES_VERSION LIMIT 10)