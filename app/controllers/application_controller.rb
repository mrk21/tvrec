class ApplicationController < ActionController::Base
  # Prevent CSRF attacks by raising an exception.
  # For APIs, you may want to use :null_session instead.
  protect_from_forgery with: :exception
  
  before_filter :check_content_type
  
  protected
  
  def check_content_type
    render text: '', layout: 'layouts/application' if params[:format].nil?
  end
end
