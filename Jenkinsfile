pipeline {
  agent any

  environment {
    PATH = "/opt/homebrew/bin:/usr/local/bin:${env.PATH}"
    VENV = "${WORKSPACE}/.venv"
    PYTHON = "${VENV}/bin/python3"
  }

  stages {
    stage('Checkout') {
      steps {
        checkout scm
      }
    }

    stage('Setup Python Env') {
      steps {
        sh """
      python3 -m venv ${VENV}
      . ${VENV}/bin/activate
      pip install --upgrade pip
      if [ -f requirements.txt ]; then pip install -r requirements.txt; fi
    """
      }
    }

    stage('Configure & Build') {
      steps {
        sh """
      . ${VENV}/bin/activate
      python3 -m scripts.build            # runs CMake configure & build
    """
      }
    }

    stage('Run Tests') {
      steps {
        sh """
      . ${VENV}/bin/activate
      python3 -m scripts.run_tests        # generates JUnit XML + logs
    """
      }
    }

    stage('Publish Results') {
      steps {
        // Parse all XMLs under test_logs/ as JUnit reports
        junit 'test_logs/*.xml'

        // Keep raw logs around
        archiveArtifacts artifacts: 'test_logs/*.log', fingerprint: true
      }
    }
  }

  post {
      always {
      echo "Pipeline finished. Build status: ${currentBuild.currentResult}"
      }
  }
}
