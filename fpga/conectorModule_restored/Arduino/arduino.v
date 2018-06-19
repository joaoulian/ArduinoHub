module arduino(out);
									 

	wire [31:0] reg_end;
	wire [31:0] reg_data;
	output [3:0] out;
	
	assign reg_end = 32'd1;
	assign reg_data = 32'd3;
	
	assign out = {reg_end[1:0], reg_data[1:0]};
	
endmodule
