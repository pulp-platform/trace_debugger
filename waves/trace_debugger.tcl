# add TRACE_IF
set tracer_if [find instances -recursive -bydu tracer_if -nodu]

if {$tracer_if ne ""} {
  add wave -group "tracer_if"                                     $tracer_if/*
}

configure wave -namecolwidth  250
configure wave -valuecolwidth 100
configure wave -justifyvalue left
configure wave -signalnamewidth 1
configure wave -timelineunits ns
