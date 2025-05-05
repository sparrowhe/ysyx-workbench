module priority_encoder_8to3 (
    input [7:0] sw,
    input en,
    output reg [2:0] code,
    output reg valid
);

always @(*) begin
    if (!en) begin
        code = 3'b000;
        valid = 0;
    end else begin
        valid = |sw;
        casez (sw)
            8'b1???????: code = 3'b111;
            8'b01??????: code = 3'b110;
            8'b001?????: code = 3'b101;
            8'b0001????: code = 3'b100;
            8'b00001???: code = 3'b011;
            8'b000001??: code = 3'b010;
            8'b0000001?: code = 3'b001;
            8'b00000001: code = 3'b000;
            default:     code = 3'b000;
        endcase
    end
end

endmodule

module seg_decoder (
    input [2:0] code,
    input valid,
    output reg [7:0] hex
);

// nvboard/example/vsrc/seg.v
wire [7:0] segs [7:0];
assign segs[0] = 8'b11111101;
assign segs[1] = 8'b01100000;
assign segs[2] = 8'b11011010;
assign segs[3] = 8'b11110010;
assign segs[4] = 8'b01100110;
assign segs[5] = 8'b10110110;
assign segs[6] = 8'b10111110;
assign segs[7] = 8'b11100000;

always @(*) begin
    if (!valid)
        hex = 8'b11111111; // 全灭
    else begin
        hex = ~segs[code];
    end
end

endmodule

module seglight (
    input [8:0] SW,         // SW[8] 为使能，SW[7:0] 为数据
    output [2:0] LED,
    output LEDVali,
    output [7:0] HEX0
);

wire [2:0] code;
wire valid;

priority_encoder_8to3 encoder (
    .sw(SW[7:0]),
    .en(SW[8]),
    .code(code),
    .valid(valid)
);

seg_decoder display (
    .code(code),
    .valid(valid),
    .hex(HEX0)
);

assign LED = code;
assign LEDVali = valid;

endmodule
