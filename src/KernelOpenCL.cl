MSTRINGIFY(

	kernel void Gol_All(
	__read_only image2d_t input,
	__write_only  image2d_t output,
	int width,
	int height
	)
{
		const sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | CLK_FILTER_NEAREST | CLK_ADDRESS_REPEAT;
		const int x = get_global_id(0);
		const int y = get_global_id(1);
		int2 cords = (int2)(x, y);
		write_imagef(output, cords, (float4)(1.0f, 1.0f, 1.0f, 1.0f));

		float4 stateAlive = (float4)(1.0f, 1.0f, 1.0f, 1.0f);
		float4 stateDead = (float4)(0.0f, 0.0f, 0.0f, 0.0f);

		int alive = 0;
		if (read_imagef(input, sampler, (int2)(x - 1, y - 1)).x > 0) alive++;
		if (read_imagef(input, sampler, (int2)(x    , y - 1)).x > 0) alive++;
		if (read_imagef(input, sampler, (int2)(x + 1, y - 1)).x > 0) alive++;

		if (read_imagef(input, sampler, (int2)(x - 1, y    )).x > 0) alive++;
		if (read_imagef(input, sampler, (int2)(x + 1, y    )).x > 0) alive++;

		if (read_imagef(input, sampler, (int2)(x - 1, y + 1)).x > 0) alive++;
		if (read_imagef(input, sampler, (int2)(x    , y + 1)).x > 0) alive++;
		if (read_imagef(input, sampler, (int2)(x + 1, y + 1)).x > 0) alive++;


		float4 color = read_imagef(input, sampler, cords);

		//calculate next state according to the amount of alive cells in the neighbourhood
		if ((alive == 3) || //a cell with 3 neigbours becomes/stays alive
			(alive == 2 && color.x > 0)) //a live cell with 2 (or 3) neighbours stays alive (with 3 neighbours it already got alive in the statement before)
			write_imagef(output, cords, stateAlive);
		else
			write_imagef(output, cords, stateDead);	//dies from overpopulation or isolation, or remains dead if it already was

	
	};

);