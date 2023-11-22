onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate -radix binary /tb_module/TB_x
add wave -noupdate -radix binary /tb_module/TB_y
add wave -noupdate -radix binary /tb_module/TB_sum
add wave -noupdate -radix binary /tb_module/TB_cout
add wave -noupdate -radix binary /tb_module/expTB_sum
add wave -noupdate -radix binary /tb_module/expTB_cout
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {0 ns} 0}
quietly wave cursor active 0
configure wave -namecolwidth 188
configure wave -valuecolwidth 100
configure wave -justifyvalue left
configure wave -signalnamewidth 0
configure wave -snapdistance 10
configure wave -datasetprefix 0
configure wave -rowmargin 4
configure wave -childrowmargin 2
configure wave -gridoffset 0
configure wave -gridperiod 1
configure wave -griddelta 40
configure wave -timeline 0
configure wave -timelineunits ns
update
WaveRestoreZoom {0 ns} {100 ns}
