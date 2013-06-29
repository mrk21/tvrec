class ChannelsController < ApplicationController
  def index
    @channels = Channel.ordered
  end
  
  def show
    @channel = Channel.find(params[:id])
  end
end
