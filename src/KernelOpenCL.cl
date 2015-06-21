MSTRINGIFY(

	kernel void Gol_All(
	__read_only image2d_t input,
	__write_only  image2d_t output,
	int width,
	int height
	)
{
		const sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | CLK_FILTER_NEAREST | CLK_ADDRESS_CLAMP;
		const int x = get_global_id(0);
		const int y = get_global_id(1);
		int2 cords = (int2)(x, y);

		const float4 stateAlive = (float4)(1.0f, 1.0f, 1.0f, 1.0f);
		const float4 stateDead = (float4)(0.0f, 0.0f, 0.0f, 0.0f);

		float4 color = read_imagef(input, sampler, cords);

		bool selfAlive = (bool)color.x;

		int alive = 0;
		if (read_imagef(input, sampler, (int2)(x - 1, y - 1)).x > 0) alive++;
		if (read_imagef(input, sampler, (int2)(x, y - 1)).x > 0) alive++;
		if (read_imagef(input, sampler, (int2)(x + 1, y - 1)).x > 0) alive++;

		if (read_imagef(input, sampler, (int2)(x - 1, y)).x > 0) alive++;
		if (read_imagef(input, sampler, (int2)(x + 1, y)).x > 0) alive++;

		if (read_imagef(input, sampler, (int2)(x - 1, y + 1)).x > 0) alive++;
		if (read_imagef(input, sampler, (int2)(x, y + 1)).x > 0) alive++;
		if (read_imagef(input, sampler, (int2)(x + 1, y + 1)).x > 0) alive++;
		
		//calculate next state according to the amount of alive cells in the neighbourhood
		if ((alive == 3) ||	((alive == 2) && selfAlive)) 
		{
			write_imagef(output, cords, stateAlive);
		}		
		else
		{
			write_imagef(output, cords, stateDead);	//dies from overpopulation or isolation, or remains dead if it already was
		}
	
	};

);