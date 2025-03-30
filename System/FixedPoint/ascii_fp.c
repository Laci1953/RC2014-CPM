void fixedtoa(int fp, char* buf)
{
	char tmpbuf[8];
	int decpart = fp & 0xFF;
	int decval = 0;

	sprintf(buf, "%d", fp >> 8);
	
	if (decpart & 0x80)
		decval += 5000;

	if (decpart & 0x40)
		decval += 2500;

	if (decpart & 0x20)
		decval += 1250;

	if (decpart & 0x10)
		decval += 625;

	if (decpart & 0x08)
		decval += 312;

	if (decpart & 0x04)
		decval += 156;

	if (decpart & 0x02)
		decval += 78;

	if (decpart & 0x01)
		decval += 39;

	if (decval)
	{
		sprintf(tmpbuf, ".%d", decval);
		strcat(buf, tmpbuf);
	}
}

//
// accepts iii[.ddd] (max 127.999)
//
int atofixed(char* p)
{
	int intp = 0;
	int dec = 0;
	int i;
	int s16 = 0;
	int f = 1000;

	i = 0;

	while (i < 3)
	{
		if (isdigit(*p))
		{
			intp = intp * 10;
			intp = intp + (*p - '0');
			p++;
		}
		else if (*p == '.')
		{
			p++;

			if (intp > 127) 
				return 0;
getdec:
			i = 0;

			while (i < 3)
			{
				if (isdigit(*p))
				{
					s16 = s16 + (*p - '0') * f;
					f = f / 10;
  					p++;
				}
				else if (*p == 0)
				{
					if (s16 == 0)
						goto decok;
					else
						break;
				}
				else
					return (0);

				i++;
			}

			if (s16 >= 5000)
			{ 
				dec = dec | 0x80;
				s16 = s16 - 5000;
			}

			if (s16 >= 2500)
			{
				dec = dec | 0x40;
				s16 = s16 - 2500;
			}

			if (s16 >= 1250)
			{
				dec = dec | 0x20;
				s16 = s16 - 1250;
			}

			if (s16 >= 625)
			{
				dec = dec | 0x10;
				s16 = s16 - 0625;
			}

			if (s16 >= 312)
			{
				dec = dec | 0x8;
				s16 = s16 - 312;
			}

			if (s16 >= 156)
			{
				dec = dec | 0x4;
				s16 = s16 - 156;
			}

			if (s16 >= 78)
			{
				dec = dec | 0x2;
				s16 = s16 - 78;
			}

			if (s16 >= 39)
				dec = dec | 0x1;
decok:
			return ((intp << 8) | dec);
		}
		else if (*p == 0)
		{
			if (intp > 127) 
				return 0;

			return (intp << 8);
		}
		else
			return 0;

		i++;
	}

	if (intp > 127) 
		return 0;

	if (*p == '.')
	{
		p++;
		goto getdec;
	}
	else if (*p == 0) 
		return (intp << 8);
	else
		return 0;
}

