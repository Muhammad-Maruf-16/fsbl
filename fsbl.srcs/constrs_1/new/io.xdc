# UART1 EMIO
set_property LOC H16 [get_ports uart_rxd]
set_property IOSTANDARD LVCMOS33 [get_ports uart_rxd]
set_property LOC H17 [get_ports uart_txd]
set_property IOSTANDARD LVCMOS33 [get_ports uart_txd]

# PUSH BUTTONS (Input, Ch 2)
# PB1(T19), PB2(P19), PB3(U20), PB4(U19)
set_property LOC T19 [get_ports {pb_tri_i[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {pb_tri_i[0]}]
set_property LOC P19 [get_ports {pb_tri_i[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {pb_tri_i[1]}]
set_property LOC U20 [get_ports {pb_tri_i[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {pb_tri_i[2]}]
set_property LOC U19 [get_ports {pb_tri_i[3]}]
set_property IOSTANDARD LVCMOS33 [get_ports {pb_tri_i[3]}]
set_property LOC V20 [get_ports {pb_tri_i[4]}]
set_property IOSTANDARD LVCMOS33 [get_ports {pb_tri_i[4]}]

# LEDS (Output, Ch 1)
# LED1(E19), LED2(K17), LED3(H18)
set_property LOC E19 [get_ports {led_tri_o[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {led_tri_o[0]}]
set_property LOC K17 [get_ports {led_tri_o[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {led_tri_o[1]}]
set_property LOC H18 [get_ports {led_tri_o[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {led_tri_o[2]}]