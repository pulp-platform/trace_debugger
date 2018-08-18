set tracer_if [find instances -recursive -bydu tracer_if -nodu]
set trace_debugger [find instances -recursive -bydu trace_debugger -nodu]

if {$tracer_if ne ""} {
    add wave -group "tracer_if" $tracer_if/*
}

if {$trace_debugger ne ""} {
    add wave -group "trace_debugger" $trace_debugger/*
}

configure wave -namecolwidth  250
configure wave -valuecolwidth 100
configure wave -justifyvalue left
configure wave -signalnamewidth 1
configure wave -timelineunits ns
