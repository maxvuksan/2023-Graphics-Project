
struct WorldSettings{

    // the chunk level where generation occurs, values are required to be ascending
    int LEVEL_AIR = 1;
    int LEVEL_SURFACE = 3;
    int LEVEL_DIRT = 8;
    int LEVEL_DIRT_TO_STONE = 9;

    int MIN_TUNNEL_SPACING = 25; // the minimum distance surface tunnels can spawn be next to each other
};