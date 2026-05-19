#!/usr/bin/env bash

set -euo pipefail

QUERY_DRIVER="${HOME}/.platformio/packages/toolchain-riscv32-esp/bin/riscv32-esp-elf-g++"

exec clangd --query-driver="${QUERY_DRIVER}" "$@"
