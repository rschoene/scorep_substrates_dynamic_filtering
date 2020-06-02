# Dynamic Filtering Plugin for Score-P

This plugin filters instrumentation calls from a binary instrumented with Score-P. Filtered calls
are overwritten with `NOP`s to minimize overhead.

This work is part of Philipp Trommlers bachelor's thesis. Results will be made available as soon as they're ready.

[![build status](https://api.travis-ci.org/rschoene/scorep_substrates_dynamic_filtering.svg)](https://travis-ci.org/rschoene/scorep_substrates_dynamic_filtering)

## Compilation and Installation

### Prerequisites

To compile this plugin, you need:

* C compiler (with `--std=c11` support)

* CMake

* Score-P installation

* `libunwind`

### Building

1. Create a build directory

        mkdir build
        cd build

2. Invoke CMake

        cmake ..

    If your Score-P installation is in a non-standard path, you have to manually pass that path to
    CMake:

        cmake .. -DSCOREP_CONFIG=<PATH_TO_YOUR_SCOREP_ROOT_DIR>/bin/scorep-config

    If you want to build the plugin with a more verbose debug output, you can invoke CMake as
    follows:

        cmake .. -DBUILD_DEBUG=on

    This plugin defaults to the identity function as the hash for `uthash` as this has proven
    slightly faster than the built-ins of `uthash`. However, if you experience performance issues
    during hash table access, you can choose one of the built-in functions as mentioned
    [here](http://troydhanson.github.io/uthash/userguide.html#hash_functions), e.g.:

        cmake .. -DHASH_FUNCTION=HASH_JEN

    By default, only the first 512 created threads are observed by the plugin. If your program uses
    more than 512 threads and you want all of them to be observed, you can increase this maximum
    by passing a higher value as `MAX_THREAD_CNT` to CMake. You can also lower the number of
    observed threads which may lead to a slightly lower overhead (but may also change the results).

        cmake .. -DMAX_THREAD_CNT=1024

3. Invoke make

        make

4. Copy the resulting library to a directory listed in `LD_LIBRARY_PATH` or add the current path to
    `LD_LIBRARY_PATH` with

        export LD_LIBRARY_PATH=`pwd`:$LD_LIBRARY_PATH

## Usage

In order to use this plugin, you have to add it to the `SCOREP_SUBSTRATES_PLUGINS` environment
variable.

    export SCOREP_SUBSTRATE_PLUGINS="dynamic_filtering"

The configuration of the plugin is done via environment variables.

### Environment variables

* `SCOREP_SUBSTRATE_DYNAMIC_FILTERING_METHOD` (string)

    Specifies the metric used by the plugin to determine the instrumentation calls to be filtered.
    Currently supported are `absolute` (filter all functions with a duration below a given
    threshold) and `relative` (filter all functions with a duration below the mean duration of all functions minus a given threshold).

* `SCOREP_SUBSTRATE_DYNAMIC_FILTERING_THRESHOLD` (integer, default 100000)

    Specifies the threshold to be used by the metrics in Score-P ticks (depends on SCOREP_TIMER)
    See `SCOREP_SUBSTRATE_DYNAMIC_FILTERING_METHOD` for details.

* `SCOREP_SUBSTRATE_DYNAMIC_FILTERING_CONTINUE_DESPITE_FAILURE` 

    If set to `true`, `True`, `TRUE`, or `1` the plugin will continue to work even though it detected, that the program has been compiled with optimizations that make re-writing impossible (see Known issues).
    
* `SCOREP_SUBSTRATE_DYNAMIC_FILTERING_CREATE_REPORT` 

    If set to `true`, `True`, `TRUE`, or `1` the plugin will write a report to stderr when finished
    
* `SCOREP_SUBSTRATE_DYNAMIC_FILTERING_CREATE_FILTER_FILE` 

    If set to `true`, `True`, `TRUE`, or `1` the plugin will write a filter file to the experiment directory
    
    
    
    
### Known issues
The compiler optimization `-foptimize-sibling-calls` is usually enabled for icc/gcc at -O2 and -O3. This will be detected by the plugin but the calls cannot be patched in this case. If you want to avoid this, but still use the other optimizations, just pass `-fno-optimize-sibling-calls` to your compiler.

The plugin only works when there is exactly one enter and one exit function for every instrumented function. This will probably not be the case with C++ programs.
### If anything fails

1. Check whether the plugin library can be loaded from the `LD_LIBRARY_PATH`

2. Check whether you provide sane values for the environment variables.

3. Open an [issue on Github](https://github.com/Ferruck/scorep_substrates_dynamic_filtering/issues).

## License

For information regarding the license of this plugin, see
[LICENSE](https://github.com/Ferruck/scorep_substrates_dynamic_filtering/blob/master/LICENSE), for
the license of `uthash` see
[uthash.LICENSE](https://github.com/Ferruck/scorep_substrates_dynamic_filtering/blob/master/uthash.LICENSE).

## Author

* Philipp Trommler (https://github.com/Ferruck)
* Robert Schoene (robert dot schoene at tu-dresden dot de)
