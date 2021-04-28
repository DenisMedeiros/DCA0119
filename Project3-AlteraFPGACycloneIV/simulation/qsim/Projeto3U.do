onerror {quit -f}
vlib work
vlog -work work Projeto3U.vo
vlog -work work Projeto3U.vt
vsim -novopt -c -t 1ps -L cycloneii_ver -L altera_ver -L altera_mf_ver -L 220model_ver -L sgate work.Projeto3U_vlg_vec_tst
vcd file -direction Projeto3U.msim.vcd
vcd add -internal Projeto3U_vlg_vec_tst/*
vcd add -internal Projeto3U_vlg_vec_tst/i1/*
add wave /*
run -all
