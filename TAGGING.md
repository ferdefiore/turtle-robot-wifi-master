# Tagging

## Fist at all: push changes to develop or master as usual

```sh
git push origin [develop|master]
```

## Develop (develop branch)

```sh
git tag -a dev-X.X.X -m 'Comment here'
git push origin release-X.X.X
```

## Production (master branch)

```sh
git tag -a release-X.X.X -m 'Comment here'
git push origin release-X.X.X
```

## Delete local tag
```sh
git tag -d release-X.X.X
```

## Delete remote tag
```sh
git push --delete origin release-X.X.X
```
