#= require_self
#= require_tree .

$$.module 'Helper', (self) ->
  self.use = (helpers...) ->
    helpers.unshift 'application'
    _(helpers).reduce (memo, helper) ->
      tree = inflection.camelize(helper).split('::')
      helper = "#{tree.pop()}Helper"
      module = ['Helper'].concat(tree).join('.')
      Helper = $$.module(module)[helper]
      _.extend(memo, new Helper())
    , {}
  
  class self.ApplicationHelper
    path: (router, action, options={}) ->
      $$.Router.path(router, action, options)
