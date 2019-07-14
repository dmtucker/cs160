# Contributing Guidelines

[GitHub](https://github.com/) hosts the project.

[![GitHub repo size](https://img.shields.io/github/repo-size/dmtucker/ogbox.svg)](https://github.com/dmtucker/ogbox)

## Development

Discuss changes by [creating an issue](https://help.github.com/articles/creating-an-issue).

[![GitHub issues](https://img.shields.io/github/issues/dmtucker/ogbox.svg)](https://github.com/dmtucker/ogbox/issues)

### Version Control

Use [`git`](https://git-scm.com/doc) to retrieve and manage the project source code.

``` sh
git clone https://github.com/dmtucker/ogbox.git
```

### Test Environment

Use [`docker`](https://docs.docker.com/) to deploy and run the project source code.

``` sh
# Build and deploy the project:
docker build -t dmtucker/ogbox:dev .
docker run -it \
  -e DISPLAY="$DISPLAY" -u "$(id -u)" -v /tmp/.X11-unix:/tmp/.X11-unix \
  dmtucker/ogbox:dev terrains/lab3.ppm textures/lab3.ppm
```

## Merges

Propose changes by [creating a pull request](https://help.github.com/articles/creating-a-pull-request/).

[![GitHub pull requests](https://img.shields.io/github/issues-pr/dmtucker/ogbox.svg)](https://github.com/dmtucker/ogbox/pulls)

- [Branch protection](https://help.github.com/articles/about-protected-branches/) enforces acceptance criteria.
- [Milestones](https://help.github.com/en/articles/about-milestones) serve as a changelog for the project.
- [Labels](https://help.github.com/en/articles/about-labels) track the intent of each change.

### Continuous Integration

[Travis CI](https://travis-ci.com/) builds, tests, and deploys the project automatically.

[![Travis (.com) branch](https://img.shields.io/travis/com/dmtucker/ogbox/master.svg)](https://travis-ci.com/dmtucker/ogbox)

## Releases

Distribute changes by [creating a release](https://help.github.com/en/articles/creating-releases).

[![GitHub release](https://img.shields.io/github/release/dmtucker/ogbox.svg)](https://github.com/dmtucker/ogbox/releases)

1. [Change the version.](http://semver.org/)
2. [Create a tag.](https://git-scm.com/book/en/v2/Git-Basics-Tagging)
3. [Release the tag.](https://help.github.com/en/articles/creating-releases)

### Package Repository

[Docker Hub](https://hub.docker.com/) distributes releases.

[![Docker Cloud Build Status](https://img.shields.io/docker/cloud/build/dmtucker/ogbox.svg)](https://cloud.docker.com/repository/docker/dmtucker/ogbox)
