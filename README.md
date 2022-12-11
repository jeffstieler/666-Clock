# The 666 Clock

7:06 is a special time. We should honor it appropriately. It's 66 minutes past the 6th hour after all.

More information to follow.

If you want to contribute add this `pre-commit` hook:

```sh
#!/bin/sh
#
# Remove absolute paths to user directories that are automatically
# written by the vscode-arduino extension.
#
# See: https://github.com/microsoft/vscode-arduino/issues/850

sed -i '' "s|$HOME/Documents/Arduino|\${workspaceRoot}|g;s|$HOME|\${HOME}|g" .vscode/c_cpp_properties.json

git add .vscode/c_cpp_properties.json
```
