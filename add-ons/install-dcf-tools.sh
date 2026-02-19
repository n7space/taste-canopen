#!/bin/bash

set -euo pipefail -o errexit

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
. "${DIR}"/common.sh

PYTHON=python3

echo "[-] Installing native system dependencies"
sudo apt-get update && sudo apt-get install python3-venv python3-pip

echo "[-] Preparing lely-core repository location"
LELY_CORE_DIR="${PREFIX}/lib/lely-core"
rm -rf "${LELY_CORE_DIR}"
mkdir -p "${LELY_CORE_DIR}"

echo "[-] Downloading lely-core"
git clone --depth 1 --branch ecss https://gitlab.com/n7space/canopen/lely-core ${LELY_CORE_DIR}

echo "[-] Setting up virtualenv for dcf-tools"
DCF_TOOLS_VENV="${PREFIX}/share/dcf-tools-venv"
rm -rf "${DCF_TOOLS_VENV}"
mkdir -p "${DCF_TOOLS_VENV}"
${PYTHON} -m venv "${DCF_TOOLS_VENV}"

echo "[-] Installing dcf-tools"
source "${DCF_TOOLS_VENV}/bin/activate"
pip install setuptools==74.1.2
pip install "${LELY_CORE_DIR}/python/dcf-tools"
deactivate

echo "[-] Adding dcf-tools to PATH"
UpdatePROFILE "export PATH=\$PATH:\"${DCF_TOOLS_VENV}\"/bin"

echo "[-] Reload terminal (or source ~/.bashrc.taste) to apply change"
