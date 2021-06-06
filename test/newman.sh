#!/bin/bash
export cwd=$(pwd)/test/collections
export APIURL=http://localhost:12345
export USERNAME=test
export EMAIL=test@localhost.com
export PASSWORD=S3cr3tP4ssw0rd
mkdir -pv $cwd
wget https://raw.githubusercontent.com/gothinkster/realworld/master/api/Conduit.postman_collection.json -O $cwd/conduit.json
docker run -v $cwd:/etc/newman -t postman/newman:alpine \
    run /etc/newman/conduit.json \
    --delay-request 500 \
    --global-var "APIURL=$APIURL" \
    --global-var "USERNAME=$USERNAME" \
    --global-var "EMAIL=$EMAIL" \
    --global-var "PASSWORD=$PASSWORD" \
    --reporters junit 
    --reporter-junit-export="newman-report.xml"