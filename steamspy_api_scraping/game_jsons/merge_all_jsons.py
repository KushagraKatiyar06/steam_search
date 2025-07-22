import os
import json

def merge_json_files():

    games = {}
    files = []
    
    print("Scanning for partial JSON files...")

    for filename in os.listdir('.'):
        
        if filename.startswith('page') and filename.endswith('.json'):
            files.append(filename)
    
    if not files:
        print("No files starting with 'page' were found.")
        return

    print(f"Found {len(files)} files to merge.")

    for filename in files:
        with open(filename, 'r', encoding='utf-8') as f:
            try:
                print(f"  > Merging {filename}...")
                data = json.load(f)
                games.update(data)
            except json.JSONDecodeError:
                print(f"    - Warning: Could not parse {filename}... Skipping.")


    output_filename = "steamspy_base_games.json"
    print(f"\nSaving {len(games)} total games to '{output_filename}'...")
    with open(output_filename, 'w', encoding='utf-8') as f:
        json.dump(games, f, indent=4)

    print("complete!")

if __name__ == "__main__":
    merge_json_files()