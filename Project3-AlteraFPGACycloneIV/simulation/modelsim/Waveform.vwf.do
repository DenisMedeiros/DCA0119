vlog -work work C:/Users/Denis/Desktop/Projeto3U/simulation/modelsim/Waveform.vwf.vt
vsim -novopt -c -t 1ps -L cycloneii_ver -L altera_ver -L altera_mf_ver -L 220model_ver -L sgate work.Projeto3U_vlg_vec_tst
onerror {resume}
add wave {Projeto3U_vlg_vec_tst/i1/CLOCK_50MHZ}
add wave {Projeto3U_vlg_vec_tst/i1/CLOCK_50KHZ}
run -all
