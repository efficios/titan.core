LTTng-UST logger plugin
=======================

This is an LTTng-UST logger plugin for the TITAN TTCN-3 Toolset.


Build
-----

To build this logger plugin:

1. Make sure [LTTng](http://lttng.org) is installed (LTTng-tools and
   LTTng-UST).
2. In your TITAN Makefile configuration, add `LTTNGUSTLOGGER := yes`.
3. Run `make` and `make install` as usual.


Use
---

To use this logger plugin with LTTng tracing:

1. Make sure the TITAN install library path is listed in your
   `LD_LIBRARY_PATH` environment variable.
2. Make sure your test builds against the _dynamic_ version
   of the TITAN runtime (with the `-dynamic` suffix).
3. In the TITAN configuration file, add the logger plugin:

        LoggerPlugins := { LTTngUSTLogger := "liblttng-ust-logger" }

4. Create an LTTng tracing session:

        lttng create

5. Enable TITAN events:

        lttng enable-event --userspace titan_core:'*'

6. Start tracing:

        lttng start

7. Start your test, and preload the TITAN runtime, as well as
   the installed tracepoint provider:

        LD_PRELOAD=libttcn3-parallel-dynamic.so:lttngust-logger-tp.so ./mytest.sh

8. When you are done, stop tracing:

        lttng stop

9. If [Babeltrace](http://diamon.org/babeltrace/) is installed, inspect the
   recorded events:

        lttng view
