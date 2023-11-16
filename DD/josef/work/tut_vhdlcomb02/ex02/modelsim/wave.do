onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate /tb_sevenseg/TB_bin
add wave -noupdate -radix binary /tb_sevenseg/TB_segments
add wave -noupdate -radix binary /tb_sevenseg/expTB_segments
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {0 ns} 0}
quietly wave cursor active 0
configure wave -namecolwidth 270
configure wave -valuecolwidth 159
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
WaveRestoreZoom {0 ns} {90 ns}
