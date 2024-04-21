import os
import random

def generate_dummy_text(length):
    """
    Generate dummy text with the specified length.

    Args:
    - length (int): The length of the dummy text in characters.

    Returns:
    - str: Dummy text.
    """
    # Define a list of words to use for generating dummy text
    words = ['Lorem', 'ipsum', 'dolor', 'sit', 'amet', 'consectetur', 'adipiscing', 'elit',
             'sed', 'do', 'eiusmod', 'tempor', 'incididunt', 'ut', 'labore', 'et', 'dolore',
             'magna', 'aliqua', 'Ut', 'enim', 'ad', 'minim', 'veniam', 'quis', 'nostrud',
             'exercitation', 'ullamco', 'laboris', 'nisi', 'ut', 'aliquip', 'ex', 'ea', 'commodo',
             'consequat', 'Duis', 'aute', 'irure', 'dolor', 'in', 'reprehenderit', 'in', 'voluptate',
             'velit', 'esse', 'cillum', 'dolore', 'eu', 'fugiat', 'nulla', 'pariatur', 'Excepteur',
             'sint', 'occaecat', 'cupidatat', 'non', 'proident', 'sunt', 'in', 'culpa', 'qui',
             'officia', 'deserunt', 'mollit', 'anim', 'id', 'est', 'laborum']

    # Generate dummy text by randomly selecting words from the list
    dummy_text = ''
    while len(dummy_text) < length:
        line_length = min(random.randint(20, 70), length - len(dummy_text))
        line = ' '.join(random.choices(words, k=line_length))
        dummy_text += line + '.\n'
    return dummy_text

def create_file(file_name, size):
    """
    Create a file with dummy text of the specified size.

    Args:
    - file_name (str): The name of the file to create.
    - size (int): The size of the file in bytes.

    Returns:
    - None
    """

    if os.path.exists(file_name):
        # If the file already exists, delete it
        os.remove(file_name)

    # Open the file in write mode
    with open(file_name, 'w') as f:
        # Write the header line
        header_text = "Project Text Editor Using Rope\n"
        f.write(header_text)

        # Write the members line
        members_text = "Members:- Dhruv Rana and Aditya Kesari\n\n"
        f.write(members_text)

        # Define the maximum chunk size to write
        max_chunk_size = 10 # You can adjust this value as needed
        
        # Write dummy text to the file until the desired size is reached
        total_bytes_written = len(header_text) + len(members_text)
        while total_bytes_written < size:
            # Calculate the chunk size to write in this iteration
            chunk_size = min(max_chunk_size, size - total_bytes_written)

            # Check if the chunk size is greater than the remaining size
            if chunk_size > size - total_bytes_written:
                # If the chunk size is greater than the remaining size,
                # write only the remaining size
                chunk_size = size - total_bytes_written
            
            # Generate dummy text for this chunk
            text = generate_dummy_text(chunk_size)
            
            # Write the text to the file
            f.write(text)
            
            # Update the total bytes written
            total_bytes_written += len(text.encode('utf-8'))

def main():
    # Get the file name and size from the user
    file_name = input("Enter the name of the file to create: ")
    size_input = input("Enter the size of the file (e.g., 1KB, 10MB, 1GB): ")

    # Convert size_input to bytes
    size_mapping = {'KB': 1024, 'MB': 1024**2, 'GB': 1024**3}
    size_str, unit = size_input[:-2], size_input[-2:].upper()
    size = int(size_str) * size_mapping[unit]

    # Create the file
    create_file(file_name, size)
    print(f"File '{file_name}' created successfully with size {os.path.getsize(file_name)}.")

if __name__ == "__main__":
    main()
