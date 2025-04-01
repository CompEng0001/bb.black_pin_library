#
# Copyright (c) 2024 Seb Blair <s.blair@gre.ac.uk>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

[ "$UID" -eq 0 ] || exec sudo bash -e "$0" "$@"

arch=$(uname -m)

if [ "x${arch}" != "xarmv7l" ] ; then
        echo ""
        echo "Error: this script: [$0] is not supported to run under [${arch}]"
        echo "-----------------------------"
        exit
fi

check_dpkg () {
        LC_ALL=C dpkg --list | awk '{print $2}' | grep "^${pkg}" >/dev/null || deb_pkgs="${deb_pkgs}${pkg} "
}

unset deb_pkgs
pkg="build-essential"
check_dpkg


if [ "${deb_pkgs}" ] ; then
        echo "Installing: ${deb_pkgs}"
        sudo apt-get update
        sudo apt-get -y install ${deb_pkgs}
        sudo apt-get clean
fi

printf "[1/3] Cleaning build files, if they exist...\n"
make clean

printf "[2/3] Uninstalling globally installed libioctrl libraries, if they exist...\nroot access requried...\n"
make uninstall

printf "[3/3] Compiling, and installing globally, root access required...\n"
sudo make install