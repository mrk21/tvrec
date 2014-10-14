if [ -d $HOME/.{{ item }}.d ]; then
  for i in $HOME/.{{ item }}.d/*.sh; do
    if [ -r $i ]; then
      . $i
    fi
  done
  unset i
fi
