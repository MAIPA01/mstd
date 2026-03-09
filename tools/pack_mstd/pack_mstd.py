import os
import shutil
import argparse
from pathlib import Path

def create_mstd_package(source_root, output_dir, version):
    source_path = Path(source_root)
    output_path = Path(output_dir)
    version_str = str(version).replace('.', '_')
    
    temp_work_dir = output_path / f"temp_mstd_build_v{version_str}"
    target_mstd_subdir = temp_work_dir / "mstd"
    zip_name = f"mstd_v{version}"
    final_zip_path = output_path / zip_name

    try:
        if temp_work_dir.exists():
            shutil.rmtree(temp_work_dir)
        target_mstd_subdir.mkdir(parents=True)

        print(f"--- Begin scaning: {source_path} ---")

        found_any = False
        for mstd_dir in source_path.glob("*/mstd/"):
            if mstd_dir.is_dir():
                print(f"copying content from: {mstd_dir}")
                for item in mstd_dir.iterdir():
                    dest = target_mstd_subdir / item.name
                    if item.is_dir():
                        shutil.copytree(item, dest, dirs_exist_ok=True)
                    else:
                        shutil.copy2(item, dest)
                found_any = True

        if not found_any:
            print("No directories matching expression '*/mstd/' found.")
            return

        print(f"Creating archive: {zip_name}.zip...")
        shutil.make_archive(
            base_name=str(final_zip_path),
            format='zip',
            root_dir=str(temp_work_dir)
        )

        print(f"Success! File saved: {final_zip_path}.zip")

    finally:
        if temp_work_dir.exists():
            shutil.rmtree(temp_work_dir)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Pakowanie zawartości folderów mstd do ZIP.")
    parser.add_argument("source", help="Ścieżka do folderu nadrzędnego")
    parser.add_argument("output", help="Ścieżka do folderu zapisu")
    parser.add_argument("version", help="Numer wersji (np. 1.0.2)")

    args = parser.parse_args()

    create_mstd_package(args.source, args.output, args.version)