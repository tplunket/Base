# Getting Started with collaboration

This document is being written for the `git` novice, someone who has experience with source control in general but can't remember how exactly to configure your setup to most effectively work with the team. These steps will get you fork set up and basic branching in place to effectively contribute to this project.

## Forking on Github

On Github, while logged into your account and in the NextAudioLabs Base repository, click the "Fork" button at the top right of the window. A "Where should we fork this repository?" window pops up, select your profile. It will work for a little while and the fork should show up in your repositories; the web page now shown will be your fork.

## Cloning from Github

Click the green "Clone or download" button to get the URL to clone from. You can press the button to the right of the URL to copy it to your system clipboard. You can use SSH instead of HTTP if you have it configured.
```
git clone -b develop https://github.com/<your-username>/Base.git
```

You'll also want to add a remote for the NextAudioLabs source.
```
git remote add NextAudioLabs https://github.com/NextAudioLabs/Base.git
```

## Committing changes locally

The NextAudioLabs repository has both a `master` and a `develop` branch. These are inspired by the [git flow](http://nvie.com/posts/a-successful-git-branching-model/) development model. Use `git flow`'s concepts if not the tools that are in many installations. There are [cheat sheets](http://danielkummer.github.io/git-flow-cheatsheet/) available as well.

At the very least:
```
git checkout -b my-feature develop
```

Using `git flow`:
```
git flow feature start my-feature
```

Make your changes in your favorite editor and commit locally as much as you like:
```
vim <files>
git add -A .
git commit -m "Creates a commit message."
```

Then merge your changes back into the develop branch. Using `git flow` again:
```
git flow feature finish my-feature
```

## Pushing changes to Github

Make sure that your `develop` branch builds and tests pass with the latest and greatest code. Sync with the `NextAudioLabs\develop` HEAD and then run the builds and the tests.
```
git pull NextAudioLabs develop
```

Once the tests pass, push the finished code up to Github.
```
git push
```

You could also push your feature branch directly to github instead of merging it into develop at this point; you'd just need to make sure to fetch from `NextAudioLabs\develop` before pushing and run the tests as you would normally.

## Creating the pull request

Using the Github UI from within your repository, click "New pull request" and from the dropdowns select `NextAudioLabs/Base` and `develop` on the left for the base fork and branch. For the head fork, choose your Base fork and the `develop` branch, unless you chose to push another branch with your changes. Choose the appropriate branch, review the changes, and click the green "Create pull request" button.

