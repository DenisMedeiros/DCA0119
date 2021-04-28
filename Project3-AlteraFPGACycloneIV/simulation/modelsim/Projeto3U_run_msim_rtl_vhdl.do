transcript on
if {[file exists rtl_work]} {
	vdel -lib rtl_work -all
}
vlib rtl_work
vmap work rtl_work

vcom -93 -work work {C:/Users/Denis/Desktop/Projeto3U/lpm_counter0.vhd}
vcom -93 -work work {C:/Users/Denis/Desktop/Projeto3U/lpm_compare0.vhd}

