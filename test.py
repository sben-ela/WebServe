import shutil

directory_to_remove = "/nfs/homes/sben-ela/WebServe/test"

try:
    shutil.rmtree(directory_to_remove)
    print(f"Directory '{directory_to_remove}' has been removed successfully.")
except FileNotFoundError:
    print(f"Directory '{directory_to_remove}' not found.")
except Exception as e:
    print(f"An error occurred: {str(e)}")
