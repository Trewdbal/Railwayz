void windowInfoTile(u8 x, u8 y)
{
	const char *txtWindowInfoTile[4];

	switch(p_world[y*WIDTH+x])
	{
		case GRASS1:
		case GRASS2:
			txtWindowInfoTile[0] = "Grassland";
			txtWindowInfoTile[1] = "";
			txtWindowInfoTile[2] = "Production: nothing";
			txtWindowInfoTile[3] = "Demand: nothing";
			break;
		case DWELLINGS1:
		case DWELLINGS2:
		case DWELLINGS3:
			txtWindowInfoTile[0] = "City";
			txtWindowInfoTile[1] = "";
			txtWindowInfoTile[2] = "Production: Passengers, mail";
			txtWindowInfoTile[3] = "Demand: Passenger, mail, food, goods";
			break;
		case FARM1:
		case FARM2:
			txtWindowInfoTile[0] = "Farm";
			txtWindowInfoTile[1] = "";
			txtWindowInfoTile[2] = "Production: Cereal";
			txtWindowInfoTile[3] = "Demand: Nothing";
			break;
		case WATER:
			txtWindowInfoTile[0] = "Water";
			txtWindowInfoTile[1] = "";
			txtWindowInfoTile[2] = "Production: Nothing";
			txtWindowInfoTile[3] = "Demand: Nothing";
			break;
		case FOREST:
			txtWindowInfoTile[0] = "Forest";
			txtWindowInfoTile[1] = "";
			txtWindowInfoTile[2] = "Production: Nothing";
			txtWindowInfoTile[3] = "Demand: Nothing";
			break;
		case LIVESTOCK:
			txtWindowInfoTile[0] = "Livestock farming";
			txtWindowInfoTile[1] = "";
			txtWindowInfoTile[2] = "Production: Livestock, wool";
			txtWindowInfoTile[3] = "Demand: Cereal";
			break;
		default:
			txtWindowInfoTile[0] = "Not yet implemented";
			txtWindowInfoTile[1] = "";
			txtWindowInfoTile[2] = "Production: ?";
			txtWindowInfoTile[3] = "Demand: ?";

	}

	drawWindow(txtWindowInfoTile, 4, 0);
}

void menuStations()
{
	u8 result;

	const char *txtMenuSizeStation[] = { 
		"Small",
		"Medium",
		"Large",
	};

	result = drawMenu(txtMenuSizeStation,3);

	switch(result)
	{
		case 0:
			CURSOR_MODE=T_SSEW;
			break;

		case 1:
			CURSOR_MODE=T_SMEW;
			break;

		case 2:
			CURSOR_MODE=T_SLEW;
			break;
	}
}


void menuTile(u8 x, u8 y)
{
	u8 menuChoice;

	const char *txtMenuTile[] = { 
		"About this tile",
		"Build a railway",
		"Build a station",
		"Destroy",
		"Railroad depot",
		"Accounting",
		"Resume",
	};

	putM2();

	do{
		menuChoice = drawMenu(txtMenuTile,7);

		if(menuChoice==0)
			windowInfoTile(x, y);
		else if(menuChoice==2)
		{
			menuStations();
			menuChoice=6;
		}

	}
	while(menuChoice!=6);

	putM1();

}

void game()
{

	int ulx = 0;
	int uly = 0;
	int xCursor = 10;
	int yCursor = 6;
	int i;
	u8 exit=0;

	cpct_clearScreen(cpct_px2byteM1(0,0,0,0));

	generateWorld();

	drawWorld(ulx, uly);

	do{
		cpct_scanKeyboard(); 

		if ( cpct_isKeyPressed(Key_CursorUp) )
		{
			drawTile(ulx, uly, xCursor, yCursor);

			yCursor-=1;

			// If cursor is out the screen
			if(yCursor<0)
			{
				yCursor=0;

				// Shift the view if needed and redraw it completely
				if(uly>0)
				{
					uly-=1;
					drawWorld(ulx, uly);
				}
			}

			// Wait loop
			for(i=0; i<14000; i++) {}
		}

		if ( cpct_isKeyPressed(Key_CursorDown) )
		{
			drawTile(ulx, uly, xCursor, yCursor);
			yCursor+=1;
			if(yCursor>NBTILE_H-1)
			{
				yCursor=NBTILE_H-1;
				if(uly<HEIGHT-NBTILE_H)
				{
					uly+=1;
					drawWorld(ulx, uly);
				}
			}

			// Wait loop
			for(i=0; i<14000; i++) {}
		}

		if ( cpct_isKeyPressed(Key_CursorLeft) )
		{
			drawTile(ulx, uly, xCursor, yCursor);
			xCursor-=1;
			if(xCursor<0)
			{
				xCursor=0;
				if(ulx>0)
				{
					ulx-=1;
					drawWorld(ulx, uly);
				}
			}

			// Wait loop
			for(i=0; i<14000; i++) {}
		}

		if ( cpct_isKeyPressed(Key_CursorRight) )
		{
			drawTile(ulx, uly, xCursor, yCursor);
			xCursor+=1;
			if(xCursor>NBTILE_W-1)
			{
				xCursor=NBTILE_W-1;
				if(ulx<WIDTH-NBTILE_W)
				{
					ulx+=1;
					drawWorld(ulx, uly);
				}
			}

			// Wait loop
			for(i=0; i<14000; i++) {}
		}


		if ( cpct_isKeyPressed(Key_Space) )
		{
			switch(CURSOR_MODE)
			{
				case T_SSNS:
					CURSOR_MODE=T_SSEW;
					break;
				case T_SSEW:
					CURSOR_MODE=T_SSNS;
					break;
				case T_SMNS:
					CURSOR_MODE=T_SMEW;
					break;
				case T_SMEW:
					CURSOR_MODE=T_SMNS;
					break;
				case T_SLNS:
					CURSOR_MODE=T_SLEW;
					break;
				case T_SLEW:
					CURSOR_MODE=T_SLNS;
					break;
			}
			// Wait loop
			for(i=0; i<14000; i++) {}
		}

		if ( cpct_isKeyPressed(Key_Esc) )
		{
			exit=1;
		}

		if ( cpct_isKeyPressed(Key_Return) )
		{
			// if standard cursor, call menu Tile
			if(CURSOR_MODE==NONE)
			{
			menuTile(ulx+xCursor, uly+yCursor);
			cpct_clearScreen(cpct_px2byteM1(0,0,0,0));	
			drawWorld(ulx, uly);
			}
			// If station cursor, apply the station tile
			else if(CURSOR_MODE>=T_SSNS && CURSOR_MODE<=T_SLEW)
			{
				p_world[ulx+xCursor+uly+yCursor*WIDTH]=CURSOR_MODE+9;
				CURSOR_MODE=NONE;
			}
			
				// Wait loop
			for(i=0; i<14000; i++) {}

		}

		drawCursor(xCursor, yCursor, 0);
	} 
	while(!exit);

}
