Tvrec::Application.routes.draw do
  root 'epgs#index'
  resources :epgs, only: :index do
    get 'in/:date', on: :collection, action: :index
    resource :reservation, only: [:create, :destroy], module: 'epgs'
  end
  resources :channels
  resources :videos
end
