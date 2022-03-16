
// This file was auto-generated as a prototype implementation of a module
// created in component editor.  It ties off all outputs to ground and
// ignores all inputs.  It needs to be edited to make it do something
// useful.
// 
// This file will not be automatically regenerated.  You should check it in
// to your version control system if you want to keep it.

`timescale 1 ps / 1 ps
module vga_onchip_interface (
		input  wire        clock_sink_clk,         //    clock_sink.clk
		input  wire        reset_sink_reset,       //    reset_sink.reset

        ///////    On-chip mem interface    //////
		input wire [17:0]   avalon_master_mem_address,  // address to on-chip-memory
		input wire          avalon_master_mem_write,    //              .write
		output  wire [7:0] avalon_master_writedata,    //              .write data

        ///////    HPS control interface    //////
        input 		[7:0]  avalon_slave_hps_addr,       //address from hps
		input              avalon_slave_hps_write,      //write command
		input       [31:0] avalon_slave_hps_writedata,  //writedata from hps
        input              avalon_slave_hps_read,       //read command
        output reg      [31:0] avalon_slave_hps_readdata    //readdata sent to hps
	);

    //////////  Control Register ///////////
    reg         ready_flag = 1'b1;  //bit [0] of 0x0
    reg         start_flag = 1'b0;  //bit [0] of 0x1
    reg [3:0]   mode_reg;           //bit [4:1] of 0x1
    reg [31:0]  control_reg_1;      //address 0x2
    reg [31:0]  control_reg_2;      //address 0x3

    /////////    HPS slave interface    //////////
    always @(posedge clock_sink_clk) begin
        if(avalon_slave_hps_write) begin
            case (avalon_slave_hps_addr)
                8'h01   :    begin
                    mode_reg <= avalon_slave_hps_writedata[4:1];
                    start_flag <= avalon_slave_hps_writedata[0];
                end
                8'h02   :    control_reg_1 <= avalon_slave_hps_writedata;
                8'h03   :    control_reg_2 <= avalon_slave_hps_writedata; 
                default :    //NOP
            endcase
        end

        else if (avalon_slave_hps_read) begin
            case (avalon_slave_hps_addr)
                8'h00   :   avalon_slave_hps_readdata <= {31'b0, ready_flag};
                8'h01   :   avalon_slave_hps_readdata <= {27'b0, mode_reg, start_flag};
                8'h02   :   avalon_slave_hps_readdata <= control_reg_1;
                8'h03   :   avalon_slave_hps_readdata <= control_reg_2;
                default :   avalon_slave_hps_readdata <= 32'b0;  //invalid addr
            endcase
        end
    end


    ////// TODO : do the first fsm
    


endmodule
