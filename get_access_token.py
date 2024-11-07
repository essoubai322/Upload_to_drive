import os
import pickle
import google.auth
from google_auth_oauthlib.flow import InstalledAppFlow
from googleapiclient.discovery import build
from googleapiclient.errors import HttpError

# If modifying or creating files, the API requires these scopes
SCOPES = ['https://www.googleapis.com/auth/drive.file']

# Path to your credentials file (downloaded as 'credentials.json')
CLIENT_SECRETS_FILE = 'credentials.json'

def authenticate():
    """Authenticate and get access token."""
    creds = None
    # The file token.pickle stores the user's access and refresh tokens.
    # It is created automatically when the authorization flow completes for the first time.
    if os.path.exists('token.pickle'):
        with open('token.pickle', 'rb') as token:
            creds = pickle.load(token)

    # If there are no valid credentials available, let the user log in.
    if not creds or not creds.valid:
        if creds and creds.expired and creds.refresh_token:
            creds.refresh(Request())
        else:
            flow = InstalledAppFlow.from_client_secrets_file(
                CLIENT_SECRETS_FILE, SCOPES)
            creds = flow.run_local_server(port=0)

        # Save the credentials for the next run
        with open('token.pickle', 'wb') as token:
            pickle.dump(creds, token)

    return creds

def upload_file(creds):
    """Uploads a file to Google Drive"""
    try:
        # Build the service
        service = build('drive', 'v3', credentials=creds)

        # Define file metadata (e.g., name and mimeType)
        file_metadata = {'name': 'example.txt'}
        media = MediaFileUpload('example.txt', mimetype='text/plain')

        # Upload file
        file = service.files().create(body=file_metadata, media_body=media, fields='id').execute()
        print('File ID: %s' % file.get('id'))

    except HttpError as error:
        print(f'An error occurred: {error}')

if __name__ == '__main__':
    creds = authenticate()
    upload_file(creds)

