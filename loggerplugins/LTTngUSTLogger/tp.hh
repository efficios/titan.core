/******************************************************************************
 * Copyright (c) 2016 Philippe Proulx
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 ******************************************************************************/

#undef TRACEPOINT_PROVIDER
#define TRACEPOINT_PROVIDER titan_core

#undef TRACEPOINT_INCLUDE
#define TRACEPOINT_INCLUDE "./tp.hh"

#if !defined(_TITAN_CORE_TP_HH) || defined(TRACEPOINT_HEADER_MULTI_READ)
#define _TITAN_CORE_TP_HH

#include <lttng/tracepoint.h>
#include <limits.h>

#define _ctf_xsd_integer(_field, _member)                               \
  ctf_integer(int, _field, static_cast<int>(_member))

#define _ctf_xsd_boolean(_field, _member)                               \
  ctf_enum(titan_core, bool, unsigned char, _field,                     \
           static_cast<boolean>(_member) ? 1 : 0)

#define _ctf_xsd_float(_field, _member)                                 \
  ctf_float(double, _field, static_cast<double>(_member))

#define _ctf_xsd_string(_field, _member)                                \
  ctf_string(_field, static_cast<const char *>(_member))

#define _ctf_xsd_enum(_enum, _field, _member)                           \
  ctf_enum(titan_core, _enum, int, _field,                              \
           (_member).is_bound() ?                                       \
             ((TitanLoggerApi::_enum::enum_type) (_member) < TitanLoggerApi::_enum::UNKNOWN_VALUE ? \
               (int) ((TitanLoggerApi::_enum::enum_type) (_member)) : INT_MAX \
             ) : INT_MAX)

#define _ctf_xsd_opt_string(_field, _member)                            \
  _ctf_xsd_integer(_field ## _present, (_member).is_present())          \
  _ctf_xsd_string(_field, (_member).is_present() ?                      \
                  static_cast<const char *>((_member)()) : "")

#define _ctf_xsd_opt_integer(_field, _member)                           \
  _ctf_xsd_integer(_field ## _present, (_member).is_present())          \
  _ctf_xsd_integer(_field, (_member).is_present() ?                     \
    static_cast<int>((_member)()) : -1)

#define _ctf_xsd_enum_invalid_value                                     \
  ctf_enum_value("(INVALID)", INT_MAX)

#define _common_fields                                                  \
  _ctf_xsd_integer(timestamp_seconds, timestamp.seconds())              \
  _ctf_xsd_integer(timestamp_microseconds, timestamp.microSeconds())    \
  _ctf_xsd_integer(severity, severity)

#define _common_args                                                    \
  const TitanLoggerApi::TimestampType&, timestamp, int, severity

TRACEPOINT_ENUM(
  titan_core,
  bool,
  TP_ENUM_VALUES(
    ctf_enum_value("false", 0)
    ctf_enum_value("true", 1)
  )
)

TRACEPOINT_ENUM(
  titan_core,
  LocationInfo_ent__type,
  TP_ENUM_VALUES(
    ctf_enum_auto("unknown")
    ctf_enum_auto("controlpart")
    ctf_enum_auto("testcase")
    ctf_enum_auto("altstep")
    ctf_enum_auto("function")
    ctf_enum_auto("external_function")
    ctf_enum_auto("template")
    _ctf_xsd_enum_invalid_value
  )
)

TRACEPOINT_EVENT(
  titan_core,
  sourceInfo,
  TP_ARGS(
    _common_args,
    const TitanLoggerApi::LocationInfo&, locationInfo
  ),
  TP_FIELDS(
    _common_fields
    _ctf_xsd_string(filename, locationInfo.filename())
    _ctf_xsd_integer(line, locationInfo.line())
    _ctf_xsd_string(ent_name, locationInfo.ent__name())
    _ctf_xsd_enum(LocationInfo_ent__type, ent_type, locationInfo.ent__type())
  )
)

TRACEPOINT_EVENT_CLASS(
  titan_core,
  Strings_class,
  TP_ARGS(
    _common_args,
    const char *, merged_string
  ),
  TP_FIELDS(
    _common_fields
    _ctf_xsd_string(logEvent_merged_strings, merged_string)
  )
)

TRACEPOINT_EVENT_INSTANCE(
  titan_core,
  Strings_class,
  actionEvent,
  TP_ARGS(
    _common_args,
    const char *, merged_string
  )
)

TRACEPOINT_ENUM(
  titan_core,
  DefaultEnd,
  TP_ENUM_VALUES(
    ctf_enum_auto("finish")
    ctf_enum_auto("repeat")
    ctf_enum_auto("break")
    _ctf_xsd_enum_invalid_value
  )
)

TRACEPOINT_EVENT_CLASS(
  titan_core,
  defaultEvent_class,
  TP_ARGS(
    _common_args,
    const TitanLoggerApi::DefaultOp&, defaultOp
  ),
  TP_FIELDS(
    _common_fields
    _ctf_xsd_string(logEvent_name, defaultOp.name())
    _ctf_xsd_integer(logEvent_id, defaultOp.id())
    _ctf_xsd_enum(DefaultEnd, logEvent_end, defaultOp.end())
  )
)

TRACEPOINT_EVENT_INSTANCE(
  titan_core,
  defaultEvent_class,
  defaultEvent_defaultopActivate,
  TP_ARGS(
    _common_args,
    const TitanLoggerApi::DefaultOp&, defaultOp
  )
)

TRACEPOINT_EVENT_INSTANCE(
  titan_core,
  defaultEvent_class,
  defaultEvent_defaultopDeactivate,
  TP_ARGS(
    _common_args,
    const TitanLoggerApi::DefaultOp&, defaultOp
  )
)

TRACEPOINT_EVENT_INSTANCE(
  titan_core,
  defaultEvent_class,
  defaultEvent_defaultopExit,
  TP_ARGS(
    _common_args,
    const TitanLoggerApi::DefaultOp&, defaultOp
  )
)

TRACEPOINT_EVENT_CLASS(
  titan_core,
  Categorized_class,
  TP_ARGS(
    _common_args,
    const TitanLoggerApi::Categorized&, event
  ),
  TP_FIELDS(
    _common_fields
    _ctf_xsd_integer(logEvent_category, event.category())
    _ctf_xsd_string(logEvent_text, event.text())
  )
)

TRACEPOINT_EVENT_INSTANCE(
  titan_core,
  Categorized_class,
  errorLog,
  TP_ARGS(
    _common_args,
    const TitanLoggerApi::Categorized&, event
  )
)

TRACEPOINT_LOGLEVEL(titan_core, errorLog, TRACE_ERR)

TRACEPOINT_ENUM(
  titan_core,
  ExecutorRuntime_reason,
  TP_ENUM_VALUES(
    ctf_enum_auto("connected_to_mc")
    ctf_enum_auto("disconnected_from_mc")
    ctf_enum_auto("initialization_of_modules_failed")
    ctf_enum_auto("exit_requested_from_mc_hc")
    ctf_enum_auto("exit_requested_from_mc_mtc")
    ctf_enum_auto("stop_was_requested_from_mc_ignored_on_idle_mtc")
    ctf_enum_auto("stop_was_requested_from_mc")
    ctf_enum_auto("stop_was_requested_from_mc_ignored_on_idle_ptc")
    ctf_enum_auto("executing_testcase_in_module")
    ctf_enum_auto("performing_error_recovery")
    ctf_enum_auto("initializing_module")
    ctf_enum_auto("initialization_of_module_finished")
    ctf_enum_auto("stopping_current_testcase")
    ctf_enum_auto("exiting")
    ctf_enum_auto("host_controller_started")
    ctf_enum_auto("host_controller_finished")
    ctf_enum_auto("stopping_control_part_execution")
    ctf_enum_auto("stopping_test_component_execution")
    ctf_enum_auto("waiting_for_ptcs_to_finish")
    ctf_enum_auto("user_paused_waiting_to_resume")
    ctf_enum_auto("resuming_execution")
    ctf_enum_auto("terminating_execution")
    ctf_enum_auto("mtc_created")
    ctf_enum_auto("overload_check")
    ctf_enum_auto("overload_check_fail")
    ctf_enum_auto("overloaded_no_more")
    ctf_enum_auto("executor_start_single_mode")
    ctf_enum_auto("executor_finish_single_mode")
    ctf_enum_auto("fd_limits")
    _ctf_xsd_enum_invalid_value
  )
)

TRACEPOINT_EVENT(
  titan_core,
  executorEvent_executorRuntime,
  TP_ARGS(
    _common_args,
    const TitanLoggerApi::ExecutorRuntime&, runtime
  ),
  TP_FIELDS(
    _common_fields
    _ctf_xsd_enum(ExecutorRuntime_reason, logEvent_reason, runtime.reason())
    _ctf_xsd_opt_string(logEvent_module_name, runtime.module__name())
    _ctf_xsd_opt_string(logEvent_testcase_name, runtime.testcase__name())
    _ctf_xsd_opt_integer(logEvent_pid, runtime.pid())
    _ctf_xsd_opt_integer(logEvent_fd_setsize, runtime.fd__setsize())
  )
)

TRACEPOINT_ENUM(
  titan_core,
  ExecutorConfigdata_reason,
  TP_ENUM_VALUES(
    ctf_enum_auto("received_from_mc")
    ctf_enum_auto("processing_failed")
    ctf_enum_auto("processing_succeeded")
    ctf_enum_auto("module_has_parameters")
    ctf_enum_auto("using_config_file")
    ctf_enum_auto("overriding_testcase_list")
    _ctf_xsd_enum_invalid_value
  )
)

TRACEPOINT_EVENT(
  titan_core,
  executorEvent_executorConfigdata,
  TP_ARGS(
    _common_args,
    const TitanLoggerApi::ExecutorConfigdata&, configData
  ),
  TP_FIELDS(
    _common_fields
    _ctf_xsd_enum(ExecutorConfigdata_reason, logEvent_reason,
                  configData.reason())
    _ctf_xsd_opt_string(logEvent_param, configData.param__())
  )
)

TRACEPOINT_EVENT(
  titan_core,
  executorEvent_extcommandStart,
  TP_ARGS(
    _common_args,
    const char *, string
  ),
  TP_FIELDS(
    _common_fields
    _ctf_xsd_string(logEvent_string, string)
  )
)

TRACEPOINT_EVENT(
  titan_core,
  executorEvent_extcommandSuccess,
  TP_ARGS(
    _common_args,
    const char *, string
  ),
  TP_FIELDS(
    _common_fields
    _ctf_xsd_string(logEvent_string, string)
  )
)

TRACEPOINT_ENUM(
  titan_core,
  ExecutorComponent_reason,
  TP_ENUM_VALUES(
    ctf_enum_auto("mtc_started")
    ctf_enum_auto("mtc_finished")
    ctf_enum_auto("ptc_started")
    ctf_enum_auto("ptc_finished")
    _ctf_xsd_enum_invalid_value
  )
)

TRACEPOINT_EVENT(
  titan_core,
  executorEvent_executorComponent,
  TP_ARGS(
    _common_args,
    const TitanLoggerApi::ExecutorComponent&, comp
  ),
  TP_FIELDS(
    _common_fields
    _ctf_xsd_enum(ExecutorComponent_reason, logEvent_reason, comp.reason())
    _ctf_xsd_opt_integer(logEvent_compref, comp.compref())
  )
)

TRACEPOINT_EVENT(
  titan_core,
  executorEvent_logOptions,
  TP_ARGS(
    _common_args,
    const char *, string
  ),
  TP_FIELDS(
    _common_fields
    _ctf_xsd_string(logEvent_string, string)
  )
)

TRACEPOINT_ENUM(
  titan_core,
  ExecutorUnqualified_reason,
  TP_ENUM_VALUES(
    ctf_enum_auto("local_address_was_set")
    ctf_enum_auto("address_of_mc_was_set")
    ctf_enum_auto("address_of_control_connection")
    ctf_enum_auto("host_support_unix_domain_sockets")
    _ctf_xsd_enum_invalid_value
  )
)

TRACEPOINT_EVENT(
  titan_core,
  executorEvent_executorMisc,
  TP_ARGS(
    _common_args,
    const TitanLoggerApi::ExecutorUnqualified&, executor
  ),
  TP_FIELDS(
    _common_fields
    _ctf_xsd_enum(ExecutorUnqualified_reason, logEvent_reason,
                  executor.reason())
    _ctf_xsd_string(logEvent_name, executor.name())
    _ctf_xsd_string(logEvent_addr, executor.addr())
    _ctf_xsd_integer(logEvent_port, executor.port__())
  )
)

TRACEPOINT_EVENT(
  titan_core,
  functionEvent_unqualified,
  TP_ARGS(
    _common_args,
    const char *, string
  ),
  TP_FIELDS(
    _common_fields
    _ctf_xsd_string(logEvent_string, string)
  )
)

TRACEPOINT_ENUM(
  titan_core,
  RandomAction,
  TP_ENUM_VALUES(
    ctf_enum_auto("seed")
    ctf_enum_auto("read_out")
    _ctf_xsd_enum_invalid_value
  )
)

TRACEPOINT_EVENT(
  titan_core,
  functionEvent_random,
  TP_ARGS(
    _common_args,
    const TitanLoggerApi::FunctionEvent_choice_random&, event
  ),
  TP_FIELDS(
    _common_fields
    _ctf_xsd_enum(RandomAction, logEvent_random_operation,
                  event.operation())
    _ctf_xsd_float(logEvent_retval, event.retval())
    _ctf_xsd_integer(logEvent_intseed, event.intseed())
  )
)

TRACEPOINT_ENUM(
  titan_core,
  ParallelPTC_reason,
  TP_ENUM_VALUES(
    ctf_enum_auto("init_component_start")
    ctf_enum_auto("init_component_finish")
    ctf_enum_auto("terminating_component")
    ctf_enum_auto("component_shut_down")
    ctf_enum_auto("error_idle_ptc")
    ctf_enum_auto("ptc_created")
    ctf_enum_auto("ptc_created_pid")
    ctf_enum_auto("function_started")
    ctf_enum_auto("function_stopped")
    ctf_enum_auto("function_finished")
    ctf_enum_auto("function_error")
    ctf_enum_auto("ptc_done")
    ctf_enum_auto("ptc_killed")
    ctf_enum_auto("stopping_mtc")
    ctf_enum_auto("ptc_stopped")
    ctf_enum_auto("all_comps_stopped")
    ctf_enum_auto("ptc_was_killed")
    ctf_enum_auto("all_comps_killed")
    ctf_enum_auto("kill_request_frm_mc")
    ctf_enum_auto("mtc_finished")
    ctf_enum_auto("ptc_finished")
    ctf_enum_auto("starting_function")
    _ctf_xsd_enum_invalid_value
  )
)

TRACEPOINT_EVENT(
  titan_core,
  parallelEvent_parallelPTC,
  TP_ARGS(
    _common_args,
    const TitanLoggerApi::ParallelPTC&, ptc
  ),
  TP_FIELDS(
    _common_fields
    _ctf_xsd_enum(ParallelPTC_reason, logEvent_reason, ptc.reason())
    _ctf_xsd_string(logEvent_module, ptc.module__())
    _ctf_xsd_string(logEvent_name, ptc.name())
    _ctf_xsd_integer(logEvent_compref, ptc.compref())
    _ctf_xsd_string(logEvent_compname, ptc.compname())
    _ctf_xsd_string(logEvent_tc_loc, ptc.tc__loc())
    _ctf_xsd_integer(logEvent_alive_pid, ptc.alive__pid())
    _ctf_xsd_integer(logEvent_status, ptc.status())
  )
)

TRACEPOINT_EVENT(
  titan_core,
  parallelEvent_parallelPTC_exit,
  TP_ARGS(
    _common_args,
    const TitanLoggerApi::PTC__exit&, ptc
  ),
  TP_FIELDS(
    _common_fields
    _ctf_xsd_integer(logEvent_compref, ptc.compref())
    _ctf_xsd_integer(logEvent_pid, ptc.pid())
    _ctf_xsd_integer(logEvent_statuscode, ptc.statuscode())
  )
)

TRACEPOINT_ENUM(
  titan_core,
  ParPort_operation,
  TP_ENUM_VALUES(
    ctf_enum_auto("connect")
    ctf_enum_auto("disconnect")
    ctf_enum_auto("map")
    ctf_enum_auto("unmap")
    _ctf_xsd_enum_invalid_value
  )
)

TRACEPOINT_EVENT(
  titan_core,
  parallelEvent_parallelPort,
  TP_ARGS(
    _common_args,
    const TitanLoggerApi::ParPort&, port
  ),
  TP_FIELDS(
    _common_fields
    _ctf_xsd_enum(ParPort_operation, logEvent_operation, port.operation())
    _ctf_xsd_integer(logEvent_srcCompref, port.srcCompref())
    _ctf_xsd_integer(logEvent_dstCompref, port.dstCompref())
    _ctf_xsd_string(logEvent_srcPort, port.srcPort())
    _ctf_xsd_string(logEvent_dstPort, port.dstPort())
  )
)

TRACEPOINT_EVENT(
  titan_core,
  testcaseOp_testcaseStarted,
  TP_ARGS(
    _common_args,
    const TitanLoggerApi::QualifiedName&, qName
  ),
  TP_FIELDS(
    _common_fields
    _ctf_xsd_string(logEvent_module_name, qName.module__name())
    _ctf_xsd_string(logEvent_testcase_name, qName.testcase__name())
  )
)

TRACEPOINT_ENUM(
  titan_core,
  Verdict,
  TP_ENUM_VALUES(
    ctf_enum_auto("v0none")
    ctf_enum_auto("v1pass")
    ctf_enum_auto("v2inconc")
    ctf_enum_auto("v3fail")
    ctf_enum_auto("v4error")
    _ctf_xsd_enum_invalid_value
  )
)

TRACEPOINT_EVENT(
  titan_core,
  testcaseOp_testcaseFinished,
  TP_ARGS(
    _common_args,
    const TitanLoggerApi::TestcaseType&, type
  ),
  TP_FIELDS(
    _common_fields
    _ctf_xsd_string(logEvent_name_module_name, type.name().module__name())
    _ctf_xsd_string(logEvent_name_testcase_name, type.name().testcase__name())
    _ctf_xsd_enum(Verdict, logEvent_verdict, type.verdict())
    _ctf_xsd_string(logEvent_reason, type.reason())
  )
)

TRACEPOINT_ENUM(
  titan_core,
  Port__Queue_operation,
  TP_ENUM_VALUES(
    ctf_enum_auto("enqueue_msg")
    ctf_enum_auto("enqueue_call")
    ctf_enum_auto("enqueue_reply")
    ctf_enum_auto("enqueue_exception")
    ctf_enum_auto("extract_msg")
    ctf_enum_auto("extract_op")
    _ctf_xsd_enum_invalid_value
  )
)

TRACEPOINT_EVENT(
  titan_core,
  portEvent_portQueue,
  TP_ARGS(
    _common_args,
    const TitanLoggerApi::Port__Queue&, queue
  ),
  TP_FIELDS(
    _common_fields
    _ctf_xsd_enum(Port__Queue_operation, logEvent_operation, queue.operation())
    _ctf_xsd_string(logEvent_port_name, queue.port__name())
    _ctf_xsd_integer(logEvent_compref, queue.compref())
    _ctf_xsd_integer(logEvent_msgid, queue.msgid())
    _ctf_xsd_string(logEvent_address, queue.address__())
    _ctf_xsd_string(logEvent_param, queue.param__())
  )
)

TRACEPOINT_ENUM(
  titan_core,
  Port__State_operation,
  TP_ENUM_VALUES(
    ctf_enum_auto("started")
    ctf_enum_auto("stopped")
    ctf_enum_auto("halted")
    _ctf_xsd_enum_invalid_value
  )
)

TRACEPOINT_EVENT(
  titan_core,
  portEvent_portState,
  TP_ARGS(
    _common_args,
    const TitanLoggerApi::Port__State&, state
  ),
  TP_FIELDS(
    _common_fields
    _ctf_xsd_enum(Port__State_operation, logEvent_operation, state.operation())
    _ctf_xsd_string(logEvent_port_name, state.port__name())
  )
)

TRACEPOINT_ENUM(
  titan_core,
  Port__oper,
  TP_ENUM_VALUES(
    ctf_enum_auto("call_op")
    ctf_enum_auto("reply_op")
    ctf_enum_auto("exception_op")
    _ctf_xsd_enum_invalid_value
  )
)

TRACEPOINT_EVENT(
  titan_core,
  portEvent_procPortSend,
  TP_ARGS(
    _common_args,
    const TitanLoggerApi::Proc__port__out&, out
  ),
  TP_FIELDS(
    _common_fields
    _ctf_xsd_string(logEvent_port_name, out.port__name())
    _ctf_xsd_enum(Port__oper, logEvent_operation, out.operation())
    _ctf_xsd_integer(logEvent_compref, out.compref())
    _ctf_xsd_string(logEvent_sys_name, out.sys__name())
    _ctf_xsd_string(logEvent_parameter, out.parameter())
  )
)

TRACEPOINT_EVENT(
  titan_core,
  portEvent_procPortRecv,
  TP_ARGS(
    _common_args,
    const TitanLoggerApi::Proc__port__in&, in
  ),
  TP_FIELDS(
    _common_fields
    _ctf_xsd_string(logEvent_port_name, in.port__name())
    _ctf_xsd_enum(Port__oper, logEvent_operation, in.operation())
    _ctf_xsd_integer(logEvent_compref, in.compref())
    _ctf_xsd_boolean(logEvent_check, in.check__())
    _ctf_xsd_string(logEvent_parameter, in.parameter())
    _ctf_xsd_integer(logEvent_msgid, in.msgid())
  )
)

TRACEPOINT_EVENT(
  titan_core,
  portEvent_msgPortSend,
  TP_ARGS(
    _common_args,
    const TitanLoggerApi::Msg__port__send&, msg
  ),
  TP_FIELDS(
    _common_fields
    _ctf_xsd_string(logEvent_port_name, msg.port__name())
    _ctf_xsd_integer(logEvent_compref, msg.compref())
    _ctf_xsd_string(logEvent_parameter, msg.parameter())
  )
)

TRACEPOINT_ENUM(
  titan_core,
  Msg__port__recv_operation,
  TP_ENUM_VALUES(
    ctf_enum_auto("receive_op")
    ctf_enum_auto("check_receive_op")
    ctf_enum_auto("trigger_op")
    _ctf_xsd_enum_invalid_value
  )
)

TRACEPOINT_EVENT(
  titan_core,
  portEvent_msgPortRecv,
  TP_ARGS(
    _common_args,
    const TitanLoggerApi::Msg__port__recv&, msg
  ),
  TP_FIELDS(
    _common_fields
    _ctf_xsd_string(logEvent_port_name, msg.port__name())
    _ctf_xsd_enum(Msg__port__recv_operation, logEvent_operation, msg.operation())
    _ctf_xsd_integer(logEvent_compref, msg.compref())
    _ctf_xsd_string(logEvent_sys_name, msg.sys__name())
    _ctf_xsd_string(logEvent_parameter, msg.parameter())
    _ctf_xsd_integer(logEvent_msgid, msg.msgid())
  )
)

TRACEPOINT_EVENT(
  titan_core,
  portEvent_dualMapped,
  TP_ARGS(
    _common_args,
    const TitanLoggerApi::Dualface__mapped&, dfm
  ),
  TP_FIELDS(
    _common_fields
    _ctf_xsd_boolean(logEvent_incoming, dfm.incoming())
    _ctf_xsd_string(logEvent_target_type, dfm.target__type())
    _ctf_xsd_string(logEvent_value, dfm.value__())
    _ctf_xsd_integer(logEvent_msgid, dfm.msgid())
  )
)

TRACEPOINT_EVENT(
  titan_core,
  portEvent_dualDiscard,
  TP_ARGS(
    _common_args,
    const TitanLoggerApi::Dualface__discard&, dfd
  ),
  TP_FIELDS(
    _common_fields
    _ctf_xsd_boolean(logEvent_incoming, dfd.incoming())
    _ctf_xsd_string(logEvent_target_type, dfd.target__type())
    _ctf_xsd_string(logEvent_port_name, dfd.port__name())
    _ctf_xsd_boolean(logEvent_unhandled, dfd.unhandled())
  )
)

TRACEPOINT_ENUM(
  titan_core,
  Port__Misc_reason,
  TP_ENUM_VALUES(
    ctf_enum_auto("removing_unterminated_connection")
    ctf_enum_auto("removing_unterminated_mapping")
    ctf_enum_auto("port_was_cleared")
    ctf_enum_auto("local_connection_established")
    ctf_enum_auto("local_connection_terminated")
    ctf_enum_auto("port_is_waiting_for_connection_tcp")
    ctf_enum_auto("port_is_waiting_for_connection_unix")
    ctf_enum_auto("connection_established")
    ctf_enum_auto("destroying_unestablished_connection")
    ctf_enum_auto("terminating_connection")
    ctf_enum_auto("sending_termination_request_failed")
    ctf_enum_auto("termination_request_received")
    ctf_enum_auto("acknowledging_termination_request_failed")
    ctf_enum_auto("sending_would_block")
    ctf_enum_auto("connection_accepted")
    ctf_enum_auto("connection_reset_by_peer")
    ctf_enum_auto("connection_closed_by_peer")
    ctf_enum_auto("port_disconnected")
    ctf_enum_auto("port_was_mapped_to_system")
    ctf_enum_auto("port_was_unmapped_from_system")
    _ctf_xsd_enum_invalid_value
  )
)

TRACEPOINT_EVENT(
  titan_core,
  portEvent_portMisc,
  TP_ARGS(
    _common_args,
    const TitanLoggerApi::Port__Misc&, misc
  ),
  TP_FIELDS(
    _common_fields
    _ctf_xsd_enum(Port__Misc_reason, logEvent_reason, misc.reason())
    _ctf_xsd_string(logEvent_port_name, misc.port__name())
    _ctf_xsd_integer(logEvent_remote_component, misc.remote__component())
    _ctf_xsd_string(logEvent_remote_port, misc.remote__port())
    _ctf_xsd_string(logEvent_ip_address, misc.ip__address())
    _ctf_xsd_integer(logEvent_tcp_port, misc.tcp__port())
    _ctf_xsd_integer(logEvent_new_size, misc.new__size())
  )
)

TRACEPOINT_EVENT(
  titan_core,
  statistics_verdictStatistics,
  TP_ARGS(
    _common_args,
    const TitanLoggerApi::StatisticsType_choice_verdictStatistics&, stats
  ),
  TP_FIELDS(
    _common_fields
    _ctf_xsd_integer(logEvent_none, stats.none__())
    _ctf_xsd_float(logEvent_nonePercent, stats.nonePercent())
    _ctf_xsd_integer(logEvent_pass, stats.pass__())
    _ctf_xsd_float(logEvent_passPercent, stats.passPercent())
    _ctf_xsd_integer(logEvent_inconc, stats.inconc__())
    _ctf_xsd_float(logEvent_inconcPercent, stats.inconcPercent())
    _ctf_xsd_integer(logEvent_fail, stats.fail__())
    _ctf_xsd_float(logEvent_failPercent, stats.failPercent())
    _ctf_xsd_integer(logEvent_error, stats.error__())
    _ctf_xsd_float(logEvent_errorPercent, stats.errorPercent())
  )
)

TRACEPOINT_EVENT(
  titan_core,
  statistics_controlpartStart,
  TP_ARGS(
    _common_args,
    const char *, string
  ),
  TP_FIELDS(
    _common_fields
    _ctf_xsd_string(logEvent_string, string)
  )
)

TRACEPOINT_EVENT(
  titan_core,
  statistics_controlpartFinish,
  TP_ARGS(
    _common_args,
    const char *, string
  ),
  TP_FIELDS(
    _common_fields
    _ctf_xsd_string(logEvent_string, string)
  )
)

TRACEPOINT_EVENT(
  titan_core,
  statistics_controlpartErrors,
  TP_ARGS(
    _common_args,
    int, number
  ),
  TP_FIELDS(
    _common_fields
    _ctf_xsd_integer(logEvent_number, number)
  )
)

TRACEPOINT_EVENT_CLASS(
  titan_core,
  TimerType_class,
  TP_ARGS(
    _common_args,
    const TitanLoggerApi::TimerType&, timer
  ),
  TP_FIELDS(
    _common_fields
    _ctf_xsd_string(logEvent_name, timer.name())
    _ctf_xsd_integer(logEvent_value, timer.value__())
  )
)

TRACEPOINT_EVENT_INSTANCE(
  titan_core,
  TimerType_class,
  timerEvent_readTimer,
  TP_ARGS(
    _common_args,
    const TitanLoggerApi::TimerType&, timer
  )
)

TRACEPOINT_EVENT_INSTANCE(
  titan_core,
  TimerType_class,
  timerEvent_startTimer,
  TP_ARGS(
    _common_args,
    const TitanLoggerApi::TimerType&, timer
  )
)

TRACEPOINT_EVENT(
  titan_core,
  timerEvent_guardTimer,
  TP_ARGS(
    _common_args,
    const TitanLoggerApi::TimerGuardType&, timer
  ),
  TP_FIELDS(
    _common_fields
    _ctf_xsd_float(logEvent_value, timer.value__())
  )
)

TRACEPOINT_EVENT_INSTANCE(
  titan_core,
  TimerType_class,
  timerEvent_stopTimer,
  TP_ARGS(
    _common_args,
    const TitanLoggerApi::TimerType&, timer
  )
)

TRACEPOINT_EVENT_INSTANCE(
  titan_core,
  TimerType_class,
  timerEvent_timeoutTimer,
  TP_ARGS(
    _common_args,
    const TitanLoggerApi::TimerType&, timer
  )
)

TRACEPOINT_EVENT(
  titan_core,
  timerEvent_timeoutAnyTimer,
  TP_ARGS(
    _common_args,
    const TitanLoggerApi::TimerAnyTimeoutType&, timer
  ),
  TP_FIELDS()
)

TRACEPOINT_EVENT(
  titan_core,
  timerEvent_unqualifiedTimer,
  TP_ARGS(
    _common_args,
    const char *, string
  ),
  TP_FIELDS(
    _common_fields
    _ctf_xsd_string(logEvent_string, string)
  )
)

TRACEPOINT_EVENT_INSTANCE(
  titan_core,
  Strings_class,
  userLog,
  TP_ARGS(
    _common_args,
    const char *, merged_string
  )
)

TRACEPOINT_EVENT(
  titan_core,
  verdictOp_setVerdict,
  TP_ARGS(
    _common_args,
    const TitanLoggerApi::SetVerdictType&, verdict
  ),
  TP_FIELDS(
    _common_fields
    _ctf_xsd_enum(Verdict, logEvent_newVerdict, verdict.newVerdict())
    _ctf_xsd_enum(Verdict, logEvent_oldVerdict, verdict.oldVerdict())
    _ctf_xsd_enum(Verdict, logEvent_localVerdict, verdict.localVerdict())
    _ctf_xsd_opt_string(logEvent_oldReason, verdict.oldReason())
    _ctf_xsd_opt_string(logEvent_newReason, verdict.newReason())
  )
)

TRACEPOINT_EVENT(
  titan_core,
  verdictOp_getVerdict,
  TP_ARGS(
    _common_args,
    const TitanLoggerApi::Verdict&, verdict
  ),
  TP_FIELDS(
    _common_fields
    _ctf_xsd_enum(Verdict, logEvent_verdict, verdict)
  )
)

TRACEPOINT_EVENT(
  titan_core,
  verdictOp_finalVerdict_info,
  TP_ARGS(
    _common_args,
    const TitanLoggerApi::FinalVerdictInfo&, info
  ),
  TP_FIELDS(
    _common_fields
    _ctf_xsd_boolean(logEvent_is_ptc, info.is__ptc())
    _ctf_xsd_enum(Verdict, logEvent_ptc_verdict, info.ptc__verdict())
    _ctf_xsd_enum(Verdict, logEvent_local_verdict, info.local__verdict())
    _ctf_xsd_enum(Verdict, logEvent_new_verdict, info.new__verdict())
    _ctf_xsd_opt_string(logEvent_verdict_reason, info.verdict__reason())
    _ctf_xsd_opt_integer(logEvent_ptc_compref, info.ptc__compref())
    _ctf_xsd_opt_string(logEvent_ptc_name, info.ptc__name())
  )
)

TRACEPOINT_ENUM(
  titan_core,
  FinalVerdictType_choice_notification,
  TP_ENUM_VALUES(
    ctf_enum_auto("setting_final_verdict_of_the_test_case.")
    ctf_enum_auto("no_ptcs_were_created.")
    _ctf_xsd_enum_invalid_value
  )
)

TRACEPOINT_EVENT(
  titan_core,
  verdictOp_finalVerdict_notification,
  TP_ARGS(
    _common_args,
    const TitanLoggerApi::FinalVerdictType_choice_notification&, notif
  ),
  TP_FIELDS(
    _common_fields
    _ctf_xsd_enum(FinalVerdictType_choice_notification, logEvent_notification, notif)
  )
)

TRACEPOINT_EVENT_INSTANCE(
  titan_core,
  Categorized_class,
  warningLog,
  TP_ARGS(
    _common_args,
    const TitanLoggerApi::Categorized&, event
  )
)

TRACEPOINT_LOGLEVEL(titan_core, warningLog, TRACE_WARNING)

TRACEPOINT_ENUM(
  titan_core,
  MatchingDoneType_reason,
  TP_ENUM_VALUES(
    ctf_enum_auto("done_failed._wrong_return_type")
    ctf_enum_auto("done_failed._no_return")
    ctf_enum_auto("any_component_done_successful")
    ctf_enum_auto("any_component_done_failed")
    ctf_enum_auto("all_component_done_successful")
    ctf_enum_auto("any_component_killed_successful")
    ctf_enum_auto("any_component_killed_failed")
    ctf_enum_auto("all_component_killed_successful")
    _ctf_xsd_enum_invalid_value
  )
)

TRACEPOINT_EVENT(
  titan_core,
  matchingEvent_matchingDone,
  TP_ARGS(
    _common_args,
    const TitanLoggerApi::MatchingDoneType&, matchingDone
  ),
  TP_FIELDS(
    _common_fields
    _ctf_xsd_enum(MatchingDoneType_reason, logEvent_reason, matchingDone.reason())
    _ctf_xsd_string(logEvent_type, matchingDone.type__())
    _ctf_xsd_integer(logEvent_ptc, matchingDone.ptc())
    _ctf_xsd_string(logEvent_return_type, matchingDone.return__type())
  )
)

TRACEPOINT_ENUM(
  titan_core,
  PortType,
  TP_ENUM_VALUES(
    ctf_enum_auto("message")
    ctf_enum_auto("procedure")
    _ctf_xsd_enum_invalid_value
  )
)

TRACEPOINT_EVENT(
  titan_core,
  matchingEvent_matchingSuccess,
  TP_ARGS(
    _common_args,
    const TitanLoggerApi::MatchingSuccessType&, matchingSuccess
  ),
  TP_FIELDS(
    _common_fields
    _ctf_xsd_enum(PortType, logEvent_port_type, matchingSuccess.port__type())
    _ctf_xsd_string(logEvent_port_name, matchingSuccess.port__name())
    _ctf_xsd_string(logEvent_info, matchingSuccess.info())
  )
)

TRACEPOINT_ENUM(
  titan_core,
  MatchingFailureType_reason,
  TP_ENUM_VALUES(
    ctf_enum_auto("sender_does_not_match_from_clause")
    ctf_enum_auto("sender_is_not_system")
    ctf_enum_auto("message_does_not_match_template")
    ctf_enum_auto("parameters_of_call_do_not_match_template")
    ctf_enum_auto("parameters_of_reply_do_not_match_template")
    ctf_enum_auto("exception_does_not_match_template")
    ctf_enum_auto("not_an_exception_for_signature")
    _ctf_xsd_enum_invalid_value
  )
)

TRACEPOINT_EVENT(
  titan_core,
  matchingEvent_matchingFailure_system,
  TP_ARGS(
    _common_args,
    const TitanLoggerApi::MatchingFailureType&, matchingFailure
  ),
  TP_FIELDS(
    _common_fields
    _ctf_xsd_enum(PortType, logEvent_port_type, matchingFailure.port__type())
    _ctf_xsd_string(logEvent_port_name, matchingFailure.port__name())
    _ctf_xsd_string(logEvent_system, matchingFailure.choice().system__())
    _ctf_xsd_enum(MatchingFailureType_reason, logEvent_reason, matchingFailure.reason())
    _ctf_xsd_string(logEvent_info, matchingFailure.info())
  )
)

TRACEPOINT_EVENT(
  titan_core,
  matchingEvent_matchingFailure_compref,
  TP_ARGS(
    _common_args,
    const TitanLoggerApi::MatchingFailureType&, matchingFailure
  ),
  TP_FIELDS(
    _common_fields
    _ctf_xsd_enum(PortType, logEvent_port_type, matchingFailure.port__type())
    _ctf_xsd_string(logEvent_port_name, matchingFailure.port__name())
    _ctf_xsd_integer(logEvent_compref, matchingFailure.choice().compref())
    _ctf_xsd_enum(MatchingFailureType_reason, logEvent_reason, matchingFailure.reason())
    _ctf_xsd_string(logEvent_info, matchingFailure.info())
  )
)

TRACEPOINT_ENUM(
  titan_core,
  MatchingProblemType_reason,
  TP_ENUM_VALUES(
    ctf_enum_auto("port_not_started_and_queue_empty")
    ctf_enum_auto("no_incoming_types")
    ctf_enum_auto("no_incoming_signatures")
    ctf_enum_auto("no_outgoing_blocking_signatures")
    ctf_enum_auto("no_outgoing_blocking_signatures_that_support_exceptions")
    ctf_enum_auto("component_has_no_ports")
    _ctf_xsd_enum_invalid_value
  )
)

TRACEPOINT_ENUM(
  titan_core,
  MatchingProblemType_operation,
  TP_ENUM_VALUES(
    ctf_enum_auto("receive")
    ctf_enum_auto("trigger")
    ctf_enum_auto("getcall")
    ctf_enum_auto("getreply")
    ctf_enum_auto("catch")
    ctf_enum_auto("check")
    _ctf_xsd_enum_invalid_value
  )
)

TRACEPOINT_EVENT(
  titan_core,
  matchingEvent_matchingProblem,
  TP_ARGS(
    _common_args,
    const TitanLoggerApi::MatchingProblemType&, matchingProblem
  ),
  TP_FIELDS(
    _common_fields
    _ctf_xsd_string(logEvent_port_name, matchingProblem.port__name())
    _ctf_xsd_enum(MatchingProblemType_reason, logEvent_reason, matchingProblem.reason())
    _ctf_xsd_enum(MatchingProblemType_operation, logEvent_operation, matchingProblem.operation())
    _ctf_xsd_boolean(logEvent_check, matchingProblem.check__())
    _ctf_xsd_boolean(logEvent_any_port, matchingProblem.any__port())
  )
)

TRACEPOINT_EVENT(
  titan_core,
  matchingEvent_matchingTimeout,
  TP_ARGS(
    _common_args,
    const TitanLoggerApi::MatchingTimeout&, matchingTimeout
  ),
  TP_FIELDS(
    _common_fields
    _ctf_xsd_opt_string(logEvent_timer_name, matchingTimeout.timer__name())
  )
)

TRACEPOINT_EVENT_INSTANCE(
  titan_core,
  Categorized_class,
  debugLog,
  TP_ARGS(
    _common_args,
    const TitanLoggerApi::Categorized&, event
  )
)

TRACEPOINT_LOGLEVEL(titan_core, debugLog, TRACE_DEBUG)

TRACEPOINT_EVENT(
  titan_core,
  executionSummary,
  TP_ARGS(
    _common_args,
    const TitanLoggerApi::ExecutionSummaryType&, summary
  ),
  TP_FIELDS(
    _common_fields
    _ctf_xsd_integer(logEvent_numberOfTestcases, summary.numberOfTestcases())
    _ctf_xsd_string(logEvent_overallStatistics, summary.overallStatistics())
  )
)

TRACEPOINT_EVENT(
  titan_core,
  unhandledEvent,
  TP_ARGS(
    _common_args,
    const char *, string
  ),
  TP_FIELDS(
    _common_fields
    _ctf_xsd_string(logEvent_string, string)
  )
)

#endif /* _TITAN_CORE_TP_HH */

#include <lttng/tracepoint-event.h>
