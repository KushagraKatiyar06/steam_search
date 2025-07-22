import json
import requests
import time
import boto3

steamspy_url = "https://steamspy.com/api.php"
details_rate_limit = 1.1

def lambda_handler(event, context):
    page = event['page']
    start_index = event['start_index']
    end_index = event['end_index']
    bucket = event['bucket_name']

    print(f"Worker starting Page {page}, Games {start_index}-{end_index} ---")
    enriched_chunk_data = {}

    try:
        summary_params = {'request': 'all', 'page': page}
        summary_response = requests.get(steamspy_url, params=summary_params)
        summary_response.raise_for_status()
        summary_data = summary_response.json()

        if not summary_data:
            return {'status': 'success', 'reason': 'empty_page'}

        
        games_on_page = list(summary_data.items())
        
        
        games_to_process = games_on_page[start_index:end_index]
        
        print(f"Processing {len(games_to_process)} games from its chunk.")
        
        for appid, summary_info in games_to_process:
            details_params = {'request': 'appdetails', 'appid': appid}
            details_response = requests.get(steamspy_url, params=details_params)
            details_response.raise_for_status()
            enriched_chunk_data[appid] = details_response.json()
            time.sleep(details_rate_limit)

        if not enriched_chunk_data:
            print("No games were processed in this chunk.")
            return {'status': 'success', 'reason': 'no_games_in_chunk'}

        s3_client = boto3.client('s3')
        
        output_filename = f"page-{page}_chunk-{start_index}-{end_index}.json"
        
        s3_client.put_object(
            Bucket=bucket,
            Key=output_filename,
            Body=json.dumps(enriched_chunk_data, indent=4)
        )

        print(f"Chunk complete. Saved to S3 as {output_filename}.")
        return {'status': 'success'}
    
    except Exception as e:
        print(f"Error on Page {page}, Chunk {start_index}-{end_index}: {e}")
        return {'status': 'error', 'error': str(e)}