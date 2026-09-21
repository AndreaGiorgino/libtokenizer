#!/usr/bin/env bash

usage='''NAME
    build - libtokenizer build script

SYNOPSIS
    build [options...] [target]

DESCRIPTION
    libtokenizer script for target builds. If no target has been provided,
    then ''build'' is used.

OPTIONS
    -v, --version
        Print the script version message and exit.

    -h, --help
        Print the script help message and exit.

    --autorun
        Autorun after build based on the target.

TARGETS
    build
        Build libtokenizer.

    debug
        Build libtokenizer in debug mode.

    test
        Build libtokenizer and tests. If ''--autorun'' has been provided, run
        the tests.

    example
        Build libtokenizer and example. If ''--autorun'' has been provided, run
        the example.
'''

version='1.0.0'

while true; do
    case $1 in
        -h | --help)
            echo "${usage}"
            exit 0
            ;;
        -v | --version)
            echo "build v${version}"
            exit 0
            ;;
        --autorun)
            autorun=true
            shift;
            ;;
        --)
            shift;
            break;
            ;;
        -*)
            echo "Invalid option '${1}'"
            echo ''
            echo "${usage}"
            exit 2
            ;;
        *)
            break;
            ;;
    esac
done

case $1 in
    '' | build)
        cmake -B build && \
            cmake --build build --parallel $(($(nproc) - 1))
        result=$?
        ;;
    debug)
        cmake -B build -DDEBUG=ON && \
            cmake --build build --parallel $(($(nproc) - 1))
        result=$?
        ;;
    test)
        cmake -B build -DLIBTOKENIZER_BUILD_TESTING=ON && \
            cmake --build build --parallel $(($(nproc) - 1))
        result=$?

        if [[ $result == 0 && $autorun ]]; then
            ctest --test-dir build --output-on-failure
            result=$?
        fi
        ;;
    example)
        cmake -B build -DLIBTOKENIZER_BUILD_EXAMPLE=ON && \
            cmake --build build --parallel $(($(nproc) - 1))
        result=$?

        if [[ $? == 0 && $autorun ]]; then
            ./build/example/example
            result=$?
        fi
        ;;
    *)
        echo "Invalid target '${1}'"
        echo ''
        echo "${usage}"
        exit 2
        ;;
esac

exit $result
