import json
import boto3

lambda_function = "steamspy-api-scraper"
bucket = "steam-games-data"
page_count = 120


def run():
    lambda_client = boto3.client('lambda')
    print(f'running {page_count} workers')

    for current_page in range(page_count):
        payload = {
            "page" : current_page,
            "bucket_name" : bucket
        }

        lambda_client.invoke(
            FunctionName = lambda_function,
            InvocationType = 'Event',
            Payload = json.dumps(payload)
        )

    print("All workers deployed..")

if __name__ == "__main__":
    run()

