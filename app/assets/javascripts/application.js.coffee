#= require URIjs/src/URI
#= require URIjs/src/URITemplate.js
#= require moment/moment
#= require inflection/lib/inflection
#= require jquery/jquery
#= require jquery_ujs
#= require underscore/underscore
#= require backbone/backbone
#= require backbone-relational/backbone-relational
#= require backbone.babysitter/lib/backbone.babysitter
#= require backbone.wreqr/lib/backbone.wreqr
#= require backbone.marionette/lib/backbone.marionette
#= require backbone-validation/dist/backbone-validation
#= require backbone-forms/distribution/backbone-forms

#= require_self
#= require_tree ./templates
#= require_tree ./routers
#= require_tree ./models
#= require_tree ./helpers
#= require_tree ./views

@$$ = new Backbone.Marionette.Application

$$.addRegions
  mainRegion: '#main_region'
  dialog_region: '#dialog_region'

$$.module('Router').on 'start', ->
  @pageView = new $$.View.Page.Layout
  $$.mainRegion.show(@pageView)
  $$.contentRegion = $$.mainRegion.currentView.bodyRegion
  @pageView.showNavView()

$$.module('Router').on 'start', ->
  Backbone.history.start(pushState: true)

$ -> $$.start()
