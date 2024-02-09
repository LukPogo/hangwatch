# Docker used to run flask server
FROM python:3.10


# Set the working directory in the container
WORKDIR /app
COPY ./ /app/

RUN pip install --upgrade pip
RUN pip install -r requirements.txt
RUN pip install gunicorn

# Deploy the flask application using gunicorn
CMD ["gunicorn", "--bind", "0.0.0.0:80", "serve:app"]