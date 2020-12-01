pipeline {
    agent {
        docker { 
            image 'empear/codescene-ci-cd:latest' 
            args '--entrypoint='
        }
    }
    stages {
        stage('Run Delta Analysis') {
            steps {
                sh '''
                  CODESCENE_DELTA_ANALYSIS_URL=http://127.0.0.1:3003/projects/20/delta-analysis
                  CODESCENE_USER=bot
                  CODESCENE_PASSWORD=secret
                  if [[ -z "$CODESCENE_DELTA_ANALYSIS_URL" ]] ; then
                    echo "No value specified for CODESCENE_DELTA_ANALYSIS_URL!"
                    exit 1
                  fi
                  if [[ -z "$CODESCENE_USER" ]] ; then
                    echo "No value specified for CODESCENE_USER!"
                    exit 1
                  fi
                  if [[ -z "$CHANGE_TARGET" ]] ; then
                    echo "No value specified for CHANGE_TARGET (must run in a PR)!"
                    exit 1
                  fi
                  if [[ -z "$CODESCENE_PASSWORD" ]] ; then
                    echo "No value specified for CODESCENE_PASSWORD!"
                    exit 1
                  fi
		  REPO_NAME=$(basename `git config --get remote.origin.url` .git)
                  codescene-ci-cd.sh \
                    --codescene-delta-analysis-url $CODESCENE_DELTA_ANALYSIS_URL \
                    --codescene-user $CODESCENE_USER \
                    --codescene-password $CODESCENE_PASSWORD \
                    --codescene-repository $REPO_NAME \
                    --fail-on-failed-goal \
                    --fail-on-declining-code-health \
                    --analyze-branch-diff \
                    --current-commit $GIT_COMMIT \
                    --base-revision origin/$CHANGE_TARGET \
                    --log-result
                    '''
            }
        }
    }
}
