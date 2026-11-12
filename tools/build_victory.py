"""Generuje assets/victory.map - lesna polana z portalem (Mission Complete)."""
import random
import sys
from pathlib import Path

W, H, TILE = 60, 34, 32
FIRST_SOLID = 598

GRASS = list(range(1, 11)) + list(range(31, 36))
FLOWERS = [93, 94, 99, 105, 112, 113, 121, 130, 131, 211, 220, 221, 236, 237]
PATH = [154, 155, 156, 157, 158, 159]
TREES = [631, 632, 635, 636, 640, 641, 661, 662, 665, 666, 670, 671, 691, 692, 695, 696]

PEDESTAL = {
    0: [None, 825, 826, 827, 828, None],
    1:,
    2:,
}
PED_X, PED_Y = 27, 12


def build():
    rng = random.Random(42)
    tiles = [[rng.choice(GRASS) for _ in range(W)] for _ in range(H)]

    # 1. Kwiatki i drzewa dookola polany
    for y in range(2, H - 2):
        for x in range(2, W - 2):
            edge_dist = min(x - 2, W - 3 - x, y - 2, H - 3 - y)
            forest_depth = 3 + rng.randint(0, 2)
            corner_dist = ((x - W / 2) / (W / 2)) ** 2 + ((y - H / 2) / (H / 2)) ** 2
            if edge_dist < forest_depth or corner_dist > 1.15:
                tiles[y][x] = rng.choice(TREES)
            elif rng.random() < 0.08:
                tiles[y][x] = rng.choice(FLOWERS)

    # 2. Kamienny plac pod portalem
    for y in range(11, 18):
        for x in range(24, 36):
            tiles[y][x] = PATH[(x - 24) % len(PATH)]

    # 3. Kamienna sciezka z dolu polany do placu
    for y in range(18, 28):
        for x in range(28, 32):
            tiles[y][x] = PATH[(x - 27) % len(PATH)]

    # 4. Zewnetrzna ramka ze scian (identyczna jak w oryginalnym skrypcie)
    for x in range(W):
        for y, base in ((0, 598), (1, 601), (H - 2, 598), (H - 1, 601)):
            tiles[y][x] = base + x % 3
    for y in range(2, H - 2):
        tiles[y][0] = 598 + y % 3
        tiles[y][1] = 601 + y % 3
        tiles[y][W - 2] = 598 + y % 3
        tiles[y][W - 1] = 601 + y % 3

    # 5. Cokol portalu na srodku
    for dy, row in PEDESTAL.items():
        for dx, tid in enumerate(row):
            if tid is not None:
                tiles[PED_Y + dy][PED_X + dx] = tid

    return tiles


def main():
    root = Path(__file__).resolve().parents[1]
    out = Path(sys.argv[1]) if len(sys.argv) > 1 else root / "assets" / "victory.map"
    tiles = build()
    lines = [f"{W} {H} {TILE} {FIRST_SOLID}"]
    lines += [" ".join(map(str, row)) for row in tiles]
    out.write_text("\n".join(lines) + "\n")
    print(f"Zapisano {out}")


if __name__ == "__main__":
    main()
