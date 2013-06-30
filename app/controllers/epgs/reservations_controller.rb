class Epgs::ReservationsController < ApplicationController
  before_filter :fetch
  
  def create
    @status = 405 unless @epg.reserve
    self.render_content
  end
  
  def destroy
    @status = 405 unless @epg.unreserve
    self.render_content
  end
  
  protected
  
  def fetch
    @epg = Epg.find(params[:epg_id])
    @status = 200
  end
  
  def render_content
    render action: :show, status: @status
  end
end
