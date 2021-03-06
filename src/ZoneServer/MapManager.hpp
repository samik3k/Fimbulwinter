#pragma once

#include  "../Common/ragnarok.hpp"
#include  "../Common/map_index.hpp"

#include <vector>

#define BLOCK_SIZE 8
#define MAX_MAP_SIZE 512 * 512

// Uncomment to activate custom cellflags like novending and nochat
// Note that this will increase map cells memory usage by 100%
//#define ENABLE_CUSTOM_CELLFLAGS

typedef enum 
{
	CELL_CHKWALL,		// wall (gat type 1)
	CELL_CHKWATER,		// water (gat type 3)
	CELL_CHKCLIFF,		// cliff/gap (gat type 5)

	CELL_CHKPASS,		// passable cell (gat type non-1/5)
	CELL_CHKREACH,		// Same as PASS, but ignores the cell-stacking mod.
	CELL_CHKNOPASS,		// non-passable cell (gat types 1 and 5)
	CELL_CHKNOREACH,	// Same as NOPASS, but ignores the cell-stacking mod.
	CELL_CHKSTACK,		// whether cell is full (reached cell stacking limit) 

	CELL_CHKNPC,
	CELL_CHKBASILICA,
	CELL_CHKLANDPROTECTOR,
	CELL_CHKNOVENDING,
	CELL_CHKNOCHAT,
	CELL_CHKMAELSTROM,
} CellCheck;

struct MapCell 
{
	// terrain flags
	unsigned char
		walkable : 1,
		shootable : 1,
		water : 1;

	// dynamic official flags
	unsigned char
		npc : 1,
		basilica : 1,
		landprotector : 1,
		maelstrom : 1;
	
	// custom flags
#ifdef ENABLE_CUSTOM_CELLFLAGS
	unsigned char
		novending : 1,
		nochat : 1,
#endif
};

enum BlockType 
{ 
	BL_NUL   = 0x000,
	BL_PC    = 0x001,
	BL_MOB   = 0x002,
	BL_PET   = 0x004,
	BL_HOM   = 0x008,
	BL_MER   = 0x010,
	BL_ITEM  = 0x020,
	BL_SKILL = 0x040,
	BL_NPC   = 0x080,
	BL_CHAT  = 0x100,
	BL_ELEM   = 0x200,

	BL_ALL   = 0xFFF,
};

struct BlockList
{
	struct BlockList *next, *prev;
	int id;
	short m, x, y;
	enum BlockType type;
};

// MapData
struct MapData 
{
	// Map name
	char name[MAP_NAME_LENGTH];

	// MapCell array
	struct MapCell* cell;

	// MapIndex ID, Width, Height(in cells)
	short m, w, h, wb, hb;

	// Number of players on the map
	int users;

	// MapFlags
	struct map_flag 
	{
		// PvP Flags
		unsigned pvp : 1;
		unsigned pvp_noparty : 1;
		unsigned pvp_noguild : 1;
		unsigned pvp_nightmaredrop :1;
		unsigned pvp_nocalcrank : 1;

		// GvG Flags
		unsigned gvg_castle : 1;
		unsigned gvg : 1;
		unsigned gvg_dungeon : 1;
		unsigned gvg_noparty : 1;
	} flags;

	// Blocks
	struct BlockList **blocks;
};

// eAthena MapCache file header
struct MapCacheHeader 
{
	unsigned int file_size;
	unsigned short map_count;
};

// eAthena MapCache Map Descriptor
struct MapCacheMapInfo 
{
	char name[MAP_NAME_LENGTH];
	unsigned short xs;
	unsigned short ys;
	unsigned int len;
};

class MapManager
{
public:
	static vector<MapData> maps;
	static map_index mapsidx;

	static void initialize();
	static bool decode_mapcache(struct MapData *m, char *buffer, char *decode_buffer);

	static int get_map_by_index(int m)
	{
		for (unsigned int i = 0; i < maps.size(); i++)
		{
			if (maps[i].m == m)
				return i;
		}

		return -1;
	}

	static bool check_cell(unsigned int m, short x, short y, CellCheck cellchk);
};
