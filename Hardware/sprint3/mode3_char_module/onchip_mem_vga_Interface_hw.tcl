# TCL File Generated by Component Editor 19.1
# Tue Mar 29 11:40:17 PDT 2022
# DO NOT MODIFY


# 
# onchip_mem_vga_Interface "onchip_vga" v1.0
#  2022.03.29.11:40:17
# Onchip Memory to VGA Interface
# 

# 
# request TCL package from ACDS 16.1
# 
package require -exact qsys 16.1


# 
# module onchip_mem_vga_Interface
# 
set_module_property DESCRIPTION "Onchip Memory to VGA Interface"
set_module_property NAME onchip_mem_vga_Interface
set_module_property VERSION 1.0
set_module_property INTERNAL false
set_module_property OPAQUE_ADDRESS_MAP true
set_module_property AUTHOR ""
set_module_property DISPLAY_NAME onchip_vga
set_module_property INSTANTIATE_IN_SYSTEM_MODULE true
set_module_property EDITABLE true
set_module_property REPORT_TO_TALKBACK false
set_module_property ALLOW_GREYBOX_GENERATION false
set_module_property REPORT_HIERARCHY false


# 
# file sets
# 
add_fileset QUARTUS_SYNTH QUARTUS_SYNTH "" ""
set_fileset_property QUARTUS_SYNTH TOP_LEVEL vga_onchip_interface
set_fileset_property QUARTUS_SYNTH ENABLE_RELATIVE_INCLUDE_PATHS false
set_fileset_property QUARTUS_SYNTH ENABLE_FILE_OVERWRITE_MODE false
add_fileset_file vga_onchip_interface.v VERILOG PATH vga_onchip_interface.v TOP_LEVEL_FILE
add_fileset_file vga_timing_module.v VERILOG PATH vga_timing_module.v


# 
# parameters
# 
add_parameter CHAR_WIDTH INTEGER 40
set_parameter_property CHAR_WIDTH DEFAULT_VALUE 40
set_parameter_property CHAR_WIDTH DISPLAY_NAME CHAR_WIDTH
set_parameter_property CHAR_WIDTH TYPE INTEGER
set_parameter_property CHAR_WIDTH UNITS None
set_parameter_property CHAR_WIDTH HDL_PARAMETER true
add_parameter CHAR_HEIGHT INTEGER 40
set_parameter_property CHAR_HEIGHT DEFAULT_VALUE 40
set_parameter_property CHAR_HEIGHT DISPLAY_NAME CHAR_HEIGHT
set_parameter_property CHAR_HEIGHT TYPE INTEGER
set_parameter_property CHAR_HEIGHT UNITS None
set_parameter_property CHAR_HEIGHT HDL_PARAMETER true
add_parameter CYAN_R STD_LOGIC_VECTOR 0
set_parameter_property CYAN_R DEFAULT_VALUE 0
set_parameter_property CYAN_R DISPLAY_NAME CYAN_R
set_parameter_property CYAN_R TYPE STD_LOGIC_VECTOR
set_parameter_property CYAN_R UNITS None
set_parameter_property CYAN_R ALLOWED_RANGES 0:511
set_parameter_property CYAN_R HDL_PARAMETER true
add_parameter CYAN_G STD_LOGIC_VECTOR 255
set_parameter_property CYAN_G DEFAULT_VALUE 255
set_parameter_property CYAN_G DISPLAY_NAME CYAN_G
set_parameter_property CYAN_G TYPE STD_LOGIC_VECTOR
set_parameter_property CYAN_G UNITS None
set_parameter_property CYAN_G ALLOWED_RANGES 0:511
set_parameter_property CYAN_G HDL_PARAMETER true
add_parameter CYAN_B STD_LOGIC_VECTOR 255
set_parameter_property CYAN_B DEFAULT_VALUE 255
set_parameter_property CYAN_B DISPLAY_NAME CYAN_B
set_parameter_property CYAN_B TYPE STD_LOGIC_VECTOR
set_parameter_property CYAN_B UNITS None
set_parameter_property CYAN_B ALLOWED_RANGES 0:511
set_parameter_property CYAN_B HDL_PARAMETER true
add_parameter PURPLE_R STD_LOGIC_VECTOR 160
set_parameter_property PURPLE_R DEFAULT_VALUE 160
set_parameter_property PURPLE_R DISPLAY_NAME PURPLE_R
set_parameter_property PURPLE_R TYPE STD_LOGIC_VECTOR
set_parameter_property PURPLE_R UNITS None
set_parameter_property PURPLE_R ALLOWED_RANGES 0:511
set_parameter_property PURPLE_R HDL_PARAMETER true
add_parameter PURPLE_G STD_LOGIC_VECTOR 32
set_parameter_property PURPLE_G DEFAULT_VALUE 32
set_parameter_property PURPLE_G DISPLAY_NAME PURPLE_G
set_parameter_property PURPLE_G TYPE STD_LOGIC_VECTOR
set_parameter_property PURPLE_G UNITS None
set_parameter_property PURPLE_G ALLOWED_RANGES 0:511
set_parameter_property PURPLE_G HDL_PARAMETER true
add_parameter PURPLE_B STD_LOGIC_VECTOR 240
set_parameter_property PURPLE_B DEFAULT_VALUE 240
set_parameter_property PURPLE_B DISPLAY_NAME PURPLE_B
set_parameter_property PURPLE_B TYPE STD_LOGIC_VECTOR
set_parameter_property PURPLE_B UNITS None
set_parameter_property PURPLE_B ALLOWED_RANGES 0:511
set_parameter_property PURPLE_B HDL_PARAMETER true


# 
# display items
# 


# 
# connection point avalon_master
# 
add_interface avalon_master avalon start
set_interface_property avalon_master addressUnits SYMBOLS
set_interface_property avalon_master associatedClock clock_sink
set_interface_property avalon_master associatedReset reset_sink
set_interface_property avalon_master bitsPerSymbol 8
set_interface_property avalon_master burstOnBurstBoundariesOnly false
set_interface_property avalon_master burstcountUnits WORDS
set_interface_property avalon_master doStreamReads false
set_interface_property avalon_master doStreamWrites false
set_interface_property avalon_master holdTime 0
set_interface_property avalon_master linewrapBursts false
set_interface_property avalon_master maximumPendingReadTransactions 0
set_interface_property avalon_master maximumPendingWriteTransactions 0
set_interface_property avalon_master readLatency 0
set_interface_property avalon_master readWaitTime 1
set_interface_property avalon_master setupTime 0
set_interface_property avalon_master timingUnits Cycles
set_interface_property avalon_master writeWaitTime 0
set_interface_property avalon_master ENABLED true
set_interface_property avalon_master EXPORT_OF ""
set_interface_property avalon_master PORT_NAME_MAP ""
set_interface_property avalon_master CMSIS_SVD_VARIABLES ""
set_interface_property avalon_master SVD_ADDRESS_GROUP ""

add_interface_port avalon_master avalon_master_address address Output 18
add_interface_port avalon_master avalon_master_read read Output 1
add_interface_port avalon_master avalon_master_readdata readdata Input 32
add_interface_port avalon_master avalon_master_waitrequest waitrequest Input 1


# 
# connection point clock_sink
# 
add_interface clock_sink clock end
set_interface_property clock_sink clockRate 0
set_interface_property clock_sink ENABLED true
set_interface_property clock_sink EXPORT_OF ""
set_interface_property clock_sink PORT_NAME_MAP ""
set_interface_property clock_sink CMSIS_SVD_VARIABLES ""
set_interface_property clock_sink SVD_ADDRESS_GROUP ""

add_interface_port clock_sink clock_sink_clk clk Input 1


# 
# connection point reset_sink
# 
add_interface reset_sink reset end
set_interface_property reset_sink associatedClock clock_sink
set_interface_property reset_sink synchronousEdges DEASSERT
set_interface_property reset_sink ENABLED true
set_interface_property reset_sink EXPORT_OF ""
set_interface_property reset_sink PORT_NAME_MAP ""
set_interface_property reset_sink CMSIS_SVD_VARIABLES ""
set_interface_property reset_sink SVD_ADDRESS_GROUP ""

add_interface_port reset_sink reset_sink_reset reset Input 1


# 
# connection point conduit_test
# 
add_interface conduit_test conduit end
set_interface_property conduit_test associatedClock clock_sink
set_interface_property conduit_test associatedReset reset_sink
set_interface_property conduit_test ENABLED true
set_interface_property conduit_test EXPORT_OF ""
set_interface_property conduit_test PORT_NAME_MAP ""
set_interface_property conduit_test CMSIS_SVD_VARIABLES ""
set_interface_property conduit_test SVD_ADDRESS_GROUP ""

add_interface_port conduit_test conduit_test new_signal_8 Output 8


# 
# connection point vga_b
# 
add_interface vga_b conduit end
set_interface_property vga_b associatedClock clock_sink
set_interface_property vga_b associatedReset ""
set_interface_property vga_b ENABLED true
set_interface_property vga_b EXPORT_OF ""
set_interface_property vga_b PORT_NAME_MAP ""
set_interface_property vga_b CMSIS_SVD_VARIABLES ""
set_interface_property vga_b SVD_ADDRESS_GROUP ""

add_interface_port vga_b VGA_B new_signal_5 Output 8


# 
# connection point vga_g
# 
add_interface vga_g conduit end
set_interface_property vga_g associatedClock clock_sink
set_interface_property vga_g associatedReset ""
set_interface_property vga_g ENABLED true
set_interface_property vga_g EXPORT_OF ""
set_interface_property vga_g PORT_NAME_MAP ""
set_interface_property vga_g CMSIS_SVD_VARIABLES ""
set_interface_property vga_g SVD_ADDRESS_GROUP ""

add_interface_port vga_g VGA_G new_signal_4 Output 8


# 
# connection point vga_r
# 
add_interface vga_r conduit end
set_interface_property vga_r associatedClock clock_sink
set_interface_property vga_r associatedReset ""
set_interface_property vga_r ENABLED true
set_interface_property vga_r EXPORT_OF ""
set_interface_property vga_r PORT_NAME_MAP ""
set_interface_property vga_r CMSIS_SVD_VARIABLES ""
set_interface_property vga_r SVD_ADDRESS_GROUP ""

add_interface_port vga_r VGA_R new_signal_2 Output 8


# 
# connection point vga_hs
# 
add_interface vga_hs conduit end
set_interface_property vga_hs associatedClock clock_sink
set_interface_property vga_hs associatedReset ""
set_interface_property vga_hs ENABLED true
set_interface_property vga_hs EXPORT_OF ""
set_interface_property vga_hs PORT_NAME_MAP ""
set_interface_property vga_hs CMSIS_SVD_VARIABLES ""
set_interface_property vga_hs SVD_ADDRESS_GROUP ""

add_interface_port vga_hs VGA_HS new_signal_1 Output 1


# 
# connection point vga_clk
# 
add_interface vga_clk conduit end
set_interface_property vga_clk associatedClock clock_sink
set_interface_property vga_clk associatedReset ""
set_interface_property vga_clk ENABLED true
set_interface_property vga_clk EXPORT_OF ""
set_interface_property vga_clk PORT_NAME_MAP ""
set_interface_property vga_clk CMSIS_SVD_VARIABLES ""
set_interface_property vga_clk SVD_ADDRESS_GROUP ""

add_interface_port vga_clk VGA_CLK new_signal Output 1


# 
# connection point vga_sync_n
# 
add_interface vga_sync_n conduit end
set_interface_property vga_sync_n associatedClock clock_sink
set_interface_property vga_sync_n associatedReset ""
set_interface_property vga_sync_n ENABLED true
set_interface_property vga_sync_n EXPORT_OF ""
set_interface_property vga_sync_n PORT_NAME_MAP ""
set_interface_property vga_sync_n CMSIS_SVD_VARIABLES ""
set_interface_property vga_sync_n SVD_ADDRESS_GROUP ""

add_interface_port vga_sync_n VGA_SYNC_N new_signal_7 Output 1


# 
# connection point vga_blank_n
# 
add_interface vga_blank_n conduit end
set_interface_property vga_blank_n associatedClock clock_sink
set_interface_property vga_blank_n associatedReset ""
set_interface_property vga_blank_n ENABLED true
set_interface_property vga_blank_n EXPORT_OF ""
set_interface_property vga_blank_n PORT_NAME_MAP ""
set_interface_property vga_blank_n CMSIS_SVD_VARIABLES ""
set_interface_property vga_blank_n SVD_ADDRESS_GROUP ""

add_interface_port vga_blank_n VGA_BLANK_N new_signal_6 Output 1


# 
# connection point vga_vs
# 
add_interface vga_vs conduit end
set_interface_property vga_vs associatedClock clock_sink
set_interface_property vga_vs associatedReset ""
set_interface_property vga_vs ENABLED true
set_interface_property vga_vs EXPORT_OF ""
set_interface_property vga_vs PORT_NAME_MAP ""
set_interface_property vga_vs CMSIS_SVD_VARIABLES ""
set_interface_property vga_vs SVD_ADDRESS_GROUP ""

add_interface_port vga_vs VGA_VS new_signal_3 Output 1


# 
# connection point avalon_slave
# 
add_interface avalon_slave avalon end
set_interface_property avalon_slave addressUnits WORDS
set_interface_property avalon_slave associatedClock clock_sink
set_interface_property avalon_slave associatedReset reset_sink
set_interface_property avalon_slave bitsPerSymbol 8
set_interface_property avalon_slave burstOnBurstBoundariesOnly false
set_interface_property avalon_slave burstcountUnits WORDS
set_interface_property avalon_slave explicitAddressSpan 0
set_interface_property avalon_slave holdTime 0
set_interface_property avalon_slave linewrapBursts false
set_interface_property avalon_slave maximumPendingReadTransactions 0
set_interface_property avalon_slave maximumPendingWriteTransactions 0
set_interface_property avalon_slave readLatency 0
set_interface_property avalon_slave readWaitTime 1
set_interface_property avalon_slave setupTime 0
set_interface_property avalon_slave timingUnits Cycles
set_interface_property avalon_slave writeWaitTime 0
set_interface_property avalon_slave ENABLED true
set_interface_property avalon_slave EXPORT_OF ""
set_interface_property avalon_slave PORT_NAME_MAP ""
set_interface_property avalon_slave CMSIS_SVD_VARIABLES ""
set_interface_property avalon_slave SVD_ADDRESS_GROUP ""

add_interface_port avalon_slave avalon_slave_addr address Input 8
add_interface_port avalon_slave avalon_slave_write write Input 1
add_interface_port avalon_slave avalon_slave_writedata writedata Input 32
set_interface_assignment avalon_slave embeddedsw.configuration.isFlash 0
set_interface_assignment avalon_slave embeddedsw.configuration.isMemoryDevice 0
set_interface_assignment avalon_slave embeddedsw.configuration.isNonVolatileStorage 0
set_interface_assignment avalon_slave embeddedsw.configuration.isPrintableDevice 0

