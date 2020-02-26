#!/bin/bash

SELFDIR=$(cd $(dirname "$1");pwd)

PROJECT_ROOT="${SELFDIR}"
while [[ ! -f "${PROJECT_ROOT}/PROJECT" ]]; do
    PROJECT_ROOT=$(dirname "${PROJECT_ROOT}")
    if [[ "/" == "${PROJECT_ROOT}" ]] || [[ "" == "${PROJECT_ROOT}" ]]; then
        exit    1
    fi
done


function format()
{
    cd ${PROJECT_ROOT}
    clang-format -i --style=file *.c *.h
    return $?
}


function main()
{
    if [[ "$1" == "format" ]]; then
        format  "$@"
        return  "$?"
    fi

    echo    "Unsupported command '$@'"
    return  2
}


main    "$@"
exit    "$?"

