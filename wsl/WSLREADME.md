# WSL Setup guide for Windows users
## Requirements
- Internet connection
- Windows 10 or 11
- Your processor must support virtualization
- Windows Core Izolation must be turned on
## Enabling wsl feature
```powershell
dism.exe /online /enable-feature /featurename:Microsoft-Windows-Subsystem-Linux /all /norestart
```
or you can simply run the following command in powershell that will do the same thing plus install default Ubuntu distro
```powershell
wsl --install
```
Unfortunately, this will install Ubuntu 20.04 if you want to install Ubuntu 22.04 you will have to do it manually by following the steps below

## Installing Ubuntu
### Checking if Ubuntu-22.04 is available
```powershell
wsl -l -o
```
If you don't see [Ubuntu 22.04](https://www.microsoft.com/store/productId/9PN20MSR04DW) in the list, you will have install it from the Microsoft Store
![Ubuntu img](Ub22.png) 

### Installing Ubuntu-22.04
```powershell
wsl --install -d Ubuntu-22.04
```
### Updating \& Upgrading Ubuntu
```bash
sudo apt update && sudo apt upgrade
```
## Installing packages
### Installing git \& python
```bash
sudo apt install git python3.10 python-is-python3 python3-pip cmake build-essential -y 
```
### Installing conan
```bash
pip3 install conan
```
### Configuring conan profile
```bash
conan profile detect --force
```
END