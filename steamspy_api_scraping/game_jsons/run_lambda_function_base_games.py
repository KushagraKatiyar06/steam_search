import json
import boto3

lambda_function = "steamspy-api-scraper"
bucket = "steam-games-data"
total_pages = 100
chunk_size = 500 

def run():
    lambda_client = boto3.client('lambda')
    
    
    print(f'Invoking {total_pages * (1000 // chunk_size)} total workers...')

    for page_number in range(total_pages):
        
        for i in range(0, 1000, chunk_size):
            start = i
            end = i + chunk_size
            
            print(f"  > Invoking worker for page {page_number}, games {start}-{end}")
            
            payload = {
                "page": page_number,
                "start_index": start,
                "end_index": end,
                "bucket_name": bucket
            }
            lambda_client.invoke(
                FunctionName=lambda_function,
                InvocationType='Event',
                Payload=json.dumps(payload)
            )
    
    print("\nAll workers have been invoked.")

if __name__ == "__main__":
    run()