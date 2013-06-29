Tvrec::Application.routes.draw do
  root 'epgs#index'
  resources :epgs, only: :index do
    get ':date', on: :collection, action: :index
  end
  resources :channels
  resources :videos
end
