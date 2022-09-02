// new_component.v

// This file was auto-generated as a prototype implementation of a module
// created in component editor.  It ties off all outputs to ground and
// ignores all inputs.  It needs to be edited to make it do something
// useful.
// 
// This file will not be automatically regenerated.  You should check it in
// to your version control system if you want to keep it.

`timescale 1 ps / 1 ps
module new_component (
		input  wire        clock_sink_clk,         //    clock_sink.clk
		input  wire        reset_sink_reset,       //    reset_sink.reset
		output wire [17:0] avalon_master_address,  // avalon_master.address
		output wire        avalon_master_read,     //              .read
		input  wire [31:0]  avalon_master_readdata, //              .readdata
		output wire [7:0]  conduit_test,            //   conduit_end.new_signal
		input  wire 	   avalon_master_waitrequest
	);

	// TODO: Auto-generated HDL template

	assign avalon_master_address = 18'b000000000000000000;

	assign avalon_master_read = 1'b1;

	assign conduit_test = avalon_master_readdata[7:0] | 8'b10000000;

endmodule
