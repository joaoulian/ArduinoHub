module arduino(out,
					in,
					arduinoResponse,
					btnClock,
					clock50,
					responseDisplay,
					data,
					address);
									 
	input btnClock;
	input clock50;
	input [1:0]data;
	input [1:0] address;
	
	parameter myNumber = 2;
	
	reg [31:0] reg_end;
	reg [31:0] reg_data;
	wire clockByTemp;
	output reg [3:0] out;
	output reg [3:0] responseDisplay;
	
	input [1:0] in;
	output reg [1:0] arduinoResponse;
	
	temporizador t (.clockin(clock50),
						 .clockout(clockByTemp));
	
	always @ (posedge clockByTemp) begin
	
		if (btnClock) begin
			reg_end <= {30'd0, address};
			reg_data <= {30'd0, data};
		end else begin
			reg_end <= 32'd0;
			reg_data <= 32'd0;
		end

		
		if (in != 2'd0) begin
			arduinoResponse <= in;
			reg_end <= myNumber;
			reg_data <= in;
		end else begin
			arduinoResponse <= 2'd0;
		end
		
		out <= {reg_end[1:0], reg_data[1:0]};
		responseDisplay <= {reg_end[1:0], reg_data[1:0]};
		
	end
		
endmodule
