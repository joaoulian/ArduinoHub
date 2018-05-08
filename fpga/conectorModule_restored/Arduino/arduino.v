module arduino(out,
					in,
					arduinoResponse);
									 

	wire [31:0] reg_end;
	wire [31:0] reg_data;
	output [3:0] out;
	
	input [1:0] in;
	output [1:0] arduinoResponse;
	
	assign reg_end = 32'd3;
	assign reg_data = 32'd2;
	
	assign out = {reg_end[1:0], reg_data[1:0]};
	assign arduinoResponse = in;
	
endmodule
