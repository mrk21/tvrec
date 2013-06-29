class EpgsController < ApplicationController
  def index
    params[:date] ||= Date.today.to_s
    @date = Date.parse(params[:date])
    @epgs = Epg.in_date(@date).order_by_time
  end
end
