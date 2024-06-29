import sys


def main():
    labels = ("text", "data", "bss", "dec", "hex", "filename")
    infos = []

    if len(sys.argv) < 3:
        print(f"Usage: {sys.argv[0]} old.siz new.siz")
        return

    for file in sys.argv[1:]:
        try:
            with open(file, "r") as f:
                data = f.read()
                line = data.split("\n")[1]
                # NOTE probably doesn't correctly handle space in name
                datas = [ele for ele in line.split("\t") if ele]

                assert len(datas) == len(labels)
                infos.append(datas)
        except:
            return

    filename = infos[0][-1]
    # Check all provided files are for the same executable
    assert all(name == filename for *_, name in infos)
    if len(infos) > 2:
        # TODO how to display this
        return

    strs = []
    for old, new, label in tuple(zip(*[*infos, labels]))[:-1]:
        old = int(old, 16 if label == "hex" else 10)
        new = int(new, 16 if label == "hex" else 10)
        delta = new-old
        strs.append(f"{new:{'x' if label == 'hex' else 'd'}} ({'+' if delta >= 0 else ''}{delta})")

    strs.append(filename)

    line1 = ""
    line2 = ""
    for label, text in zip(labels, strs):
        length = max(len(label), len(text)) + 0
        line1 += f"{label:<{length}s}\t"
        line2 += f"{text:<{length}s}\t"

    print(line1.strip())
    print(line2.strip())


if __name__ == "__main__":
    main()
