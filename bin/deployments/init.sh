#!/bin/sh

EXEC_USER="tvrec"
NAME="Unicorn"
ENV=production

ROOT_DIR="/home/tvrec/tvrec"

PID="${ROOT_DIR}/tmp/pids/unicorn.pid"
CONF="${ROOT_DIR}/config/unicorn.rb"
OPTIONS=""
CMD="${ROOT_DIR}/bin/bundle exec unicorn_rails -c ${CONF} -E ${ENV} -D ${OPTIONS}"

start()
{
  if [ -e $PID ]; then
    echo "$NAME already started"
    exit 1
  fi
  echo "start $NAME"
  su - ${EXEC_USER} -c "cd ${ROOT_DIR} && ${CMD}"
}

stop()
{
  if [ ! -e $PID ]; then
    echo "$NAME not started"
    exit 1
  fi
  echo "stop $NAME"
  su - ${EXEC_USER} -c "kill -QUIT $(cat ${PID})"
}

force_stop()
{
  if [ ! -e $PID ]; then
    echo "$NAME not started"
    exit 1
  fi
  echo "stop $NAME"
  su - ${EXEC_USER} -c "kill -INT $(cat ${PID})"
}

reload()
{
  if [ ! -e $PID ]; then
    echo "$NAME not started"
    start
    exit 0
  fi
  echo "reload $NAME"
  su - ${EXEC_USER} -c "kill -HUP $(cat ${PID})"
}

restart()
{
    stop
    # Unicorn が停止し切らない内に起動しようとしないように
    sleep 3
    start
}

case "$1" in
  start)
    start
    ;;
  stop)
    stop
    ;;
  force-stop)
    force_stop
    ;;
  reload)
    reload
    ;;
  restart)
    restart
    ;;
  *)
    echo "Syntax Error: release [start|stop|force-stop|reload|restart]"
    ;;
esac

