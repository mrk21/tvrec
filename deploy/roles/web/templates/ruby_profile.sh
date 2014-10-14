export RBENV_ROOT={{ ruby_rbenv_path }}
export PATH=${RBENV_ROOT}/bin:${PATH}
eval "$(rbenv init --no-rehash -)"
