# TCL File Generated by Component Editor 19.1
# Thu Mar 17 00:02:10 PDT 2022
# DO NOT MODIFY


# 
# render_accelerator "render_accelerator" v1.0
#  2022.03.17.00:02:10
# 
# 

# 
# request TCL package from ACDS 16.1
# 
package require -exact qsys 16.1


# 
# module render_accelerator
# 
set_module_property DESCRIPTION ""
set_module_property NAME render_accelerator
set_module_property VERSION 1.0
set_module_property INTERNAL false
set_module_property OPAQUE_ADDRESS_MAP true
set_module_property AUTHOR ""
set_module_property DISPLAY_NAME render_accelerator
set_module_property INSTANTIATE_IN_SYSTEM_MODULE true
set_module_property EDITABLE true
set_module_property REPORT_TO_TALKBACK false
set_module_property ALLOW_GREYBOX_GENERATION false
set_module_property REPORT_HIERARCHY false


# 
# file sets
# 
add_fileset QUARTUS_SYNTH QUARTUS_SYNTH "" ""
set_fileset_property QUARTUS_SYNTH TOP_LEVEL render_accelerator_module
set_fileset_property QUARTUS_SYNTH ENABLE_RELATIVE_INCLUDE_PATHS false
set_fileset_property QUARTUS_SYNTH ENABLE_FILE_OVERWRITE_MODE false
add_fileset_file render_accelerator_module.v VERILOG PATH render_accelerator_module.v TOP_LEVEL_FILE


# 
# parameters
# 
add_parameter idle STD_LOGIC_VECTOR 0
set_parameter_property idle DEFAULT_VALUE 0
set_parameter_property idle DISPLAY_NAME idle
set_parameter_property idle TYPE STD_LOGIC_VECTOR
set_parameter_property idle ENABLED false
set_parameter_property idle UNITS None
set_parameter_property idle ALLOWED_RANGES 0:63
set_parameter_property idle HDL_PARAMETER true
add_parameter inc_x STD_LOGIC_VECTOR 17
set_parameter_property inc_x DEFAULT_VALUE 17
set_parameter_property inc_x DISPLAY_NAME inc_x
set_parameter_property inc_x TYPE STD_LOGIC_VECTOR
set_parameter_property inc_x ENABLED false
set_parameter_property inc_x UNITS None
set_parameter_property inc_x ALLOWED_RANGES 0:63
set_parameter_property inc_x HDL_PARAMETER true
add_parameter inc_y STD_LOGIC_VECTOR 18
set_parameter_property inc_y DEFAULT_VALUE 18
set_parameter_property inc_y DISPLAY_NAME inc_y
set_parameter_property inc_y TYPE STD_LOGIC_VECTOR
set_parameter_property inc_y ENABLED false
set_parameter_property inc_y UNITS None
set_parameter_property inc_y ALLOWED_RANGES 0:63
set_parameter_property inc_y HDL_PARAMETER true
add_parameter finish STD_LOGIC_VECTOR 3
set_parameter_property finish DEFAULT_VALUE 3
set_parameter_property finish DISPLAY_NAME finish
set_parameter_property finish TYPE STD_LOGIC_VECTOR
set_parameter_property finish ENABLED false
set_parameter_property finish UNITS None
set_parameter_property finish ALLOWED_RANGES 0:63
set_parameter_property finish HDL_PARAMETER true
add_parameter square STD_LOGIC_VECTOR 0
set_parameter_property square DEFAULT_VALUE 0
set_parameter_property square DISPLAY_NAME square
set_parameter_property square TYPE STD_LOGIC_VECTOR
set_parameter_property square ENABLED false
set_parameter_property square UNITS None
set_parameter_property square ALLOWED_RANGES 0:31
set_parameter_property square HDL_PARAMETER true


# 
# display items
# 


# 
# connection point avalon_slave_hps
# 
add_interface avalon_slave_hps avalon end
set_interface_property avalon_slave_hps addressUnits WORDS
set_interface_property avalon_slave_hps associatedClock clock_sink
set_interface_property avalon_slave_hps associatedReset reset_sink
set_interface_property avalon_slave_hps bitsPerSymbol 8
set_interface_property avalon_slave_hps burstOnBurstBoundariesOnly false
set_interface_property avalon_slave_hps burstcountUnits WORDS
set_interface_property avalon_slave_hps explicitAddressSpan 0
set_interface_property avalon_slave_hps holdTime 0
set_interface_property avalon_slave_hps linewrapBursts false
set_interface_property avalon_slave_hps maximumPendingReadTransactions 0
set_interface_property avalon_slave_hps maximumPendingWriteTransactions 0
set_interface_property avalon_slave_hps readLatency 0
set_interface_property avalon_slave_hps readWaitTime 1
set_interface_property avalon_slave_hps setupTime 0
set_interface_property avalon_slave_hps timingUnits Cycles
set_interface_property avalon_slave_hps writeWaitTime 0
set_interface_property avalon_slave_hps ENABLED true
set_interface_property avalon_slave_hps EXPORT_OF ""
set_interface_property avalon_slave_hps PORT_NAME_MAP ""
set_interface_property avalon_slave_hps CMSIS_SVD_VARIABLES ""
set_interface_property avalon_slave_hps SVD_ADDRESS_GROUP ""

add_interface_port avalon_slave_hps avalon_slave_hps_write write Input 1
add_interface_port avalon_slave_hps avalon_slave_hps_writedata writedata Input 32
add_interface_port avalon_slave_hps avalon_slave_hps_read read Input 1
add_interface_port avalon_slave_hps avalon_slave_hps_readdata readdata Output 32
add_interface_port avalon_slave_hps avalon_slave_hps_addr address Input 8
set_interface_assignment avalon_slave_hps embeddedsw.configuration.isFlash 0
set_interface_assignment avalon_slave_hps embeddedsw.configuration.isMemoryDevice 0
set_interface_assignment avalon_slave_hps embeddedsw.configuration.isNonVolatileStorage 0
set_interface_assignment avalon_slave_hps embeddedsw.configuration.isPrintableDevice 0


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

add_interface_port avalon_master avalon_master_mem_address address Output 18
add_interface_port avalon_master avalon_master_mem_write write Output 1
add_interface_port avalon_master avalon_master_mem_writedata writedata Output 8
add_interface_port avalon_master avalon_master_waitrequest waitrequest Input 1

