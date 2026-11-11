import argparse
import io
import math
import random
import zipfile
from collections import deque
from pathlib import Path

from PIL import Image


TILE = 32
WIDTH, HEIGHT = 120, 72
SPAWN = (19, 49)
ROOT = Path(__file__).resolve().parents[1]


def build(archive, output, preview):
    rng = random.Random(1705)
    with zipfile.ZipFile(archive) as source:
        sheets = {
            name: Image.open(io.BytesIO(source.read(name + ".png"))).convert("RGBA")
            for name in ("Grass", "Grass 2 layer", "Wall", "Plants", "Props")
        }

    def crop(name, x, y, w=1, h=1):
        return sheets[name].crop((x * TILE, y * TILE, (x + w) * TILE, (y + h) * TILE))

    ground = crop("Grass", 0, 0)
    world = Image.new("RGBA", (WIDTH * TILE, HEIGHT * TILE))
    blocked = [[False] * WIDTH for _ in range(HEIGHT)]
    reserved = [[False] * WIDTH for _ in range(HEIGHT)]
    paths = set()

    for y in range(HEIGHT):
        for x in range(WIDTH):
            world.paste(ground, (x * TILE, y * TILE))

    for y in range(1, HEIGHT - 2, 2):
        for x in range(1, WIDTH - 2, 2):
            if rng.random() < 0.3:
                patch = crop("Grass", 1, 0, 2, 2)
                if rng.choice((False, True)):
                    patch = patch.transpose(Image.Transpose.FLIP_LEFT_RIGHT)
                world.alpha_composite(patch, (x * TILE, y * TILE))

    for y in range(HEIGHT):
        for x in range(WIDTH):
            if rng.random() < 0.35:
                tuft = crop("Grass 2 layer", rng.randrange(4, 10), rng.choice((3, 4, 5, 6, 7)))
                world.alpha_composite(tuft, (x * TILE, y * TILE))

    def reserve(x, y, w, h, margin=0):
        for ty in range(max(0, y - margin), min(HEIGHT, y + h + margin)):
            for tx in range(max(0, x - margin), min(WIDTH, x + w + margin)):
                reserved[ty][tx] = True

    def stamp(sprite, x, y, solid=False):
        world.alpha_composite(sprite, (x * TILE, y * TILE))
        if solid:
            for sy in range(math.ceil(sprite.height / TILE)):
                for sx in range(math.ceil(sprite.width / TILE)):
                    alpha = sprite.getchannel("A").crop((sx * TILE, sy * TILE, (sx + 1) * TILE, (sy + 1) * TILE))
                    if sum(alpha.histogram()[64:]) > 64 and 0 <= y + sy < HEIGHT and 0 <= x + sx < WIDTH:
                        blocked[y + sy][x + sx] = True

    def plaza(x, y, w, h):
        reserve(x, y, w, h, 2)
        for ty in range(y, y + h):
            for tx in range(x, x + w):
                paths.add((tx, ty))

    def path(points, radius=2):
        for (ax, ay), (bx, by) in zip(points, points[1:]):
            steps = max(abs(bx - ax), abs(by - ay))
            for i in range(steps + 1):
                x, y = round(ax + (bx - ax) * i / max(1, steps)), round(ay + (by - ay) * i / max(1, steps))
                for ty in range(y - radius, y + radius + 1):
                    for tx in range(x - radius, x + radius + 1):
                        if 2 <= tx < WIDTH - 2 and 2 <= ty < HEIGHT - 2:
                            paths.add((tx, ty))
                reserve(x - radius, y - radius, radius * 2 + 1, radius * 2 + 1, 2)

    plaza(13, 44, 14, 12)
    plaza(13, 19, 14, 10)
    plaza(46, 32, 23, 18)
    plaza(89, 17, 19, 19)
    plaza(93, 53, 15, 11)
    path([(19, 49), (35, 49), (35, 42), (57, 42), (79, 42), (79, 26), (98, 26)])
    path([(19, 49), (19, 24)])
    path([(57, 42), (57, 60), (100, 60), (100, 57)])
    path([(57, 42), (57, 18), (38, 18), (38, 10), (20, 10), (20, 24)], 1)
    path([(98, 26), (98, 44), (105, 44), (105, 57)], 1)

    paving = [crop("Grass 2 layer", x, y) for x in range(10, 16) for y in (0, 1, 2, 4, 5, 13)]
    floor = [crop("Wall", x, 2) for x in range(8, 13)]
    for x, y in sorted(paths):
        inside = all(cell in paths for cell in ((x - 1, y), (x + 1, y), (x, y - 1), (x, y + 1)))
        stamp(rng.choice(floor if inside and rng.random() < 0.97 else paving), x, y)

    for x in range(WIDTH):
        for y in (0, 1, HEIGHT - 2, HEIGHT - 1):
            stamp(crop("Wall", 36 + x % 6, 7 + y % 2), x, y)
            blocked[y][x] = True
    for y in range(2, HEIGHT - 2):
        for x in (0, 1, WIDTH - 2, WIDTH - 1):
            stamp(crop("Wall", 36 + y % 6, 7 + x % 2), x, y)
            blocked[y][x] = True
    reserve(0, 0, WIDTH, 2)
    reserve(0, HEIGHT - 2, WIDTH, 2)
    reserve(0, 0, 2, HEIGHT)
    reserve(WIDTH - 2, 0, 2, HEIGHT)

    def horizontal_wall(x, y, length, gaps=()):
        reserve(x, y, length, 3, 1)
        for dx in range(length):
            if any(a <= x + dx < b for a, b in gaps):
                continue
            stamp(crop("Wall", 2 + dx % 3, 6, 1, 3), x + dx, y)
            for dy in range(3):
                blocked[y + dy][x + dx] = True

    def vertical_wall(x, y, length, gaps=()):
        reserve(x, y, 2, length, 1)
        for dy in range(length):
            if any(a <= y + dy < b for a, b in gaps):
                continue
            stamp(crop("Wall", 2 + (dy % 2), 5, 2, 1), x, y + dy)
            blocked[y + dy][x] = blocked[y + dy][x + 1] = True

    horizontal_wall(10, 15, 21, ((17, 23),))
    horizontal_wall(10, 30, 21, ((16, 23),))
    vertical_wall(10, 18, 12)
    vertical_wall(29, 18, 12, ((23, 27),))
    horizontal_wall(43, 28, 30, ((54, 61),))
    horizontal_wall(43, 52, 30, ((54, 61),))
    vertical_wall(43, 31, 21, ((39, 46),))
    vertical_wall(71, 31, 21, ((39, 46),))
    horizontal_wall(85, 12, 27)
    horizontal_wall(85, 37, 27, ((95, 102),))
    vertical_wall(85, 15, 22, ((23, 30),))
    vertical_wall(110, 15, 22)

    objects = [
        (crop("Props", 0, 0, 2, 4), 14, 46),
        (crop("Props", 4, 1, 2, 3), 25, 45),
        (crop("Props", 6, 6, 2, 2), 24, 53),
        (crop("Props", 2, 1, 2, 3), 13, 19),
        (crop("Props", 4, 1, 2, 3), 25, 20),
        (crop("Props", 6, 6, 2, 2), 24, 26),
        (crop("Props", 6, 0, 2, 6), 47, 32),
        (crop("Props", 9, 0, 2, 6), 65, 32),
        (crop("Props", 0, 0, 2, 4), 47, 46),
        (crop("Props", 0, 0, 2, 4), 66, 46),
        (crop("Props", 0, 5, 6, 3), 95, 18),
        (crop("Props", 6, 0, 2, 6), 89, 17),
        (crop("Props", 9, 0, 2, 6), 105, 17),
        (crop("Props", 0, 0, 2, 4), 90, 31),
        (crop("Props", 0, 0, 2, 4), 105, 31),
        (crop("Props", 10, 6, 4, 3), 98, 54),
        (crop("Props", 6, 6, 2, 2), 94, 61),
        (crop("Props", 6, 6, 2, 2), 106, 61),
    ]
    stamp(crop("Props", 7, 8, 3, 2), 56, 37)
    stamp(crop("Props", 7, 8, 3, 2), 97, 24)
    for sprite, x, y in objects:
        reserve(x, y, math.ceil(sprite.width / TILE), math.ceil(sprite.height / TILE), 1)

    trees = [crop("Plants", x, 0, w, 7) for x, w in ((0, 4), (4, 4), (8, 3), (11, 3), (14, 3), (17, 4))]
    for _ in range(1700):
        tree = rng.choice(trees)
        x, y = rng.randrange(3, WIDTH - 7), rng.randrange(2, HEIGHT - 9)
        w, h = tree.width // TILE, tree.height // TILE
        if any(reserved[ty][tx] for ty in range(y, y + h) for tx in range(x, x + w)):
            continue
        objects.append((tree, x, y))
        reserve(x, y, w, h)

    bushes = [crop("Plants", x, 1, 2, 2) for x in (21, 23, 25, 27, 29)]
    for _ in range(650):
        x, y = rng.randrange(3, WIDTH - 4), rng.randrange(3, HEIGHT - 4)
        if any(reserved[ty][tx] for ty in range(y, y + 2) for tx in range(x, x + 2)):
            continue
        objects.append((rng.choice(bushes), x, y))
        reserve(x, y, 2, 2)

    for x0, y0, w, h in ((6, 36, 8, 6), (27, 56, 10, 9), (62, 7, 13, 12), (80, 53, 9, 12), (102, 6, 10, 4)):
        for y in range(y0, y0 + h):
            for x in range(x0, x0 + w):
                if not reserved[y][x] and rng.random() < 0.7:
                    stamp(crop("Grass 2 layer", rng.randrange(0, 4), rng.choice((1, 2, 8, 11, 15))), x, y)

    for sprite, x, y in sorted(objects, key=lambda obj: obj[2] * TILE + obj[0].height):
        stamp(sprite, x, y, solid=True)

    def can_stand(x, y):
        return 1 <= x < WIDTH - 1 and 1 <= y < HEIGHT - 1 and not any(
            blocked[ty][tx] for ty in range(y - 1, y + 2) for tx in range(x - 1, x + 2)
        )

    assert can_stand(*SPAWN), "Spawn is blocked"
    visited = {SPAWN}
    queue = deque([SPAWN])
    while queue:
        x, y = queue.popleft()
        for cell in ((x - 1, y), (x + 1, y), (x, y - 1), (x, y + 1)):
            if cell not in visited and can_stand(*cell):
                visited.add(cell)
                queue.append(cell)
    for target in ((19, 24), (57, 42), (98, 26), (104, 60), (38, 18)):
        assert target in visited, f"Area at {target} is unreachable"

    world = world.convert("RGB")
    unique = [{}, {}]
    cells = []
    for y in range(HEIGHT):
        for x in range(WIDTH):
            tile = world.crop((x * TILE, y * TILE, (x + 1) * TILE, (y + 1) * TILE))
            group = int(blocked[y][x])
            key = tile.tobytes()
            if key not in unique[group]:
                unique[group][key] = (len(unique[group]), tile)
            cells.append((group, unique[group][key][0]))
    first_solid = len(unique[0])
    count = first_solid + len(unique[1])
    atlas = Image.new("RGB", (32 * TILE, math.ceil(count / 32) * TILE))
    for group in (0, 1):
        for index, tile in unique[group].values():
            tile_id = index + (first_solid if group else 0)
            atlas.paste(tile, ((tile_id % 32) * TILE, (tile_id // 32) * TILE))

    output.mkdir(parents=True, exist_ok=True)
    atlas.save(output / "tileset.bmp")
    ids = [index + (first_solid if group else 0) for group, index in cells]
    rows = [" ".join(map(str, ids[y * WIDTH:(y + 1) * WIDTH])) for y in range(HEIGHT)]
    (output / "world.map").write_text(f"{WIDTH} {HEIGHT} {TILE} {first_solid}\n" + "\n".join(rows) + "\n", encoding="ascii")
    preview.parent.mkdir(parents=True, exist_ok=True)
    world.save(preview)
    print(f"World: {world.width}x{world.height}, atlas: {atlas.width}x{atlas.height}, {count} tiles ({first_solid} passable)")
    print(f"Spawn: (624, 1584); {len(visited)} reachable cells with a 3x3 clearance; all five landmarks reachable")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Build the 32px world atlas from the original Resurrected RPG 1.1 ZIP. Requires Pillow; the game only needs the generated BMP and MAP files. The MAP header is width height tile_size first_solid_tile, followed by row-major tile IDs.")
    parser.add_argument("archive", type=Path)
    parser.add_argument("--output", type=Path, default=ROOT / "assets")
    parser.add_argument("--preview", type=Path, default=ROOT / "build" / "map-preview.png")
    args = parser.parse_args()
    build(args.archive, args.output, args.preview)
